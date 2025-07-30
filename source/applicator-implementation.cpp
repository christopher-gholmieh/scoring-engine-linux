// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/configuration-implementation.hpp"
#include "vulnerabilities/permission-implementation.hpp"
#include "vulnerabilities/forensics-implementation.hpp"
#include "vulnerabilities/package-implementation.hpp"
#include "vulnerabilities/service-implementation.hpp"
#include "vulnerabilities/group-implementation.hpp"
#include "vulnerabilities/user-implementation.hpp"
#include "vulnerabilities/file-implementation.hpp"

#include "vulnerability-implementation.hpp"

#include "applicator-implementation.hpp"
#include "filesystem-utilities.hpp"

// Core:
#include <iostream>
#include <fstream>
#include <sstream>

#include <memory>
#include <vector>


// Functions:
std::string read_text_file(const std::string &path) {
    // Variables (Assignment):
    // Stream:
    std::ifstream stream {path};
    
    if (!stream.is_open()) {
        throw std::runtime_error("[!] Failed to open file: " + path + "!");
    }

    // Buffer:
    std::ostringstream buffer;

    // Logic:
    buffer << stream.rdbuf();

    return buffer.str();
}

static std::string generate_operating_system_label() {
    // Variables (Assignment):
    // Buffer:
    char buffer[128];

    // Pipe:
    FILE* pipe = popen("uname -r", "r");

    if (pipe == nullptr) {
        return "Unknown";
    }

    // Result:
    std::string result = "Unknown";

    // Logic:
    if (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        result = buffer;

        if (result.empty() == false && result.back() == '\n') {
            result.pop_back();
        }
    }

    // Memory:
    pclose(pipe);

    // Logic:
    return result;
}

static void apply_forensics(const std::string &path, const std::string &question) {
    // Variables (Assignment):
    // File:
    std::ofstream file(path);

    // Instructions:
    file << "Instructions:\n\n";
    file << "1) Type the answer to the question in place of\n";
    file << "<Type Answer Here>\n\n";
    file << "2) Save the file (you will not get credit until the\n";
    file << "file is saved)\n\n";
    file << "Example question (not scored):\n\n";
    file << "What was the name of the Lone Ranger's horse?\n\n";
    file << "ANSWER: <Type Answer Here>\n\n";
    file << "After correctly answering the question, the example\n";
    file << "question would look like this:\n\n";
    file << "What was the name of the Lone Ranger's horse?\n\n";
    file << "ANSWER: Silver\n\n";
    file << "The scored question appears below.\n\n";
    
    // Divider:
    file << "============================================================" << "\n\n";

    // Question:
    file << question << "\n\n";

    // Answer:
    file << "ANSWER: <Type Answer Here>" << "\n" << std::endl;

    // Memory:
    file.close();
}

static void apply_package(const std::string &package, package_behavior_t package_behavior) {
    // Variables (Assignment):
    // Command:
    char command[512];

    // Logic:
    if (package_behavior == PACKAGE_INSTALLED) {
        snprintf(command, sizeof(command), "apt remove -y %s", package.c_str());

        if (system(command) != 0) {
            std::cout << "[!] Failed to uninstall package " + package + "!" << std::endl;
        }
    } else if (package_behavior == PACKAGE_REMOVED) {
        snprintf(command, sizeof(command), "apt install -y %s", package.c_str());

        if (system(command) != 0) {
            std::cout << "[!] Failed to install package " + package + "!" << std::endl;
        }
    } else if (package_behavior == PACKAGE_UPDATED) {
        std::cout << "[!] There is no current support for package update vulnerabilities.!" << std::endl;
    }
}

static void apply_service(const std::string &service, const service_behavior_t service_behavior) {
    // Variables (Assignment):
    // Command:
    std::string command;

    // Logic:
    switch (service_behavior) {
        case ENABLE_SERVICE: {
            // Command:
            command = "systemctl disable --now " + service;

            // Logic:
            if (system(command.c_str()) != 0) {
                std::cout << "[!] Failed to disable service " + service + "!" << std::endl;
            }

            break;
        }

        case DISABLE_SERVICE: {
            // Command:
            command = "systemctl enable --now " + service;

            // Logic:
            if (system(command.c_str()) != 0) {
                std::cout << "[!] Failed to enable service " + service + "!" << std::endl;
            }

            break;
        }
    }
}

static void apply_group(const std::string &user, const std::string &group, const group_behavior_t group_behavior) {
    // Variables (Assignment):
    // Command:
    std::string command;

    // Logic:
    switch (group_behavior) {
        case USER_IN_GROUP_NOT: {
            // Command:
            command = "usermod -aG " + group + " " + user;

            // Logic:
            if (system(command.c_str()) != 0) {
                // Log:
                std::cout << "[!] Failed to add user " + user + " to group " + group + "!" << std::endl;

                // Logic:
                std::cout << "[*] Retrying!" << std::endl;

                // Variables (Assignment:)
                // Command:
                std::string group_command = "groupadd " + group;

                // Logic:
                if (system(group_command.c_str()) != 0) {
                    std::cout << "[!] Failed to create group " + group << std::endl;
                }

                if (system(command.c_str()) != 0) {
                    std::cout << "[!] Retry failed!" << std::endl;
                }
            }

            break;
        }

        case USER_IN_GROUP: {
            // Command:
            command = "gpasswd -d " + user + " " + group;

            // Logic:
            if (system(command.c_str()) != 0) {
                std::cout << "[!] Failed to remove user " + user + " from group " + group + "!" << std::endl;
            }

            break;
        }
    }
}

static void apply_user(const std::string &user, const user_behavior_t user_behavior) {
    // Variables (Assignment):
    // Command:
    std::string command;

    // Logic:
    switch (user_behavior) {
        case CREATE_USER: {
            // Command:
            command = "userdel " + user;

            // Logic:
            if (system(command.c_str()) == 0) {
                std::cout << "[!] Removed user " + user + "!" << std::endl;
            }

            break;
        }

        case DELETE_USER: {
            // Command:
            command = "useradd " + user;

            // Logic:
            if (system(command.c_str()) == 0) {
                std::cout << "[!] Created user " + user + "!" << std::endl;
            }

            break;
        }
    }
}

static void apply_permission(const std::string &path, const std::string &permissions) {
	// Variables (Assignment):
	// Command:
	const std::string command = "chmod " + permissions + " " + path;

	// Logic:
	if (system(command.c_str()) != 0) {
		std::cout << "[!] Failed to apply permission " + permissions + " to " + path + "!" << std::endl;
	}
}

static void applicator_logic(std::vector<std::unique_ptr<Vulnerability>> &&vector) {
    for (const auto &vulnerability: vector) {
        // Forensics:
        if (auto forensics_vulnerability = dynamic_cast<Forensics*>(vulnerability.get())) {
            apply_forensics(
                /* Path: */
                forensics_vulnerability->get_path(),

                /* Question: */
                forensics_vulnerability->get_question()
            );
        } else if (auto package_vulnerability = dynamic_cast<Package*>(vulnerability.get())) {
            apply_package(
                /* Package: */
                package_vulnerability->get_package(),

                /* Behavior: */
                package_vulnerability->get_package_behavior()
            );
        } else if (auto service_vulnerability = dynamic_cast<Service*>(vulnerability.get())) {
            apply_service(
                /* Service: */
                service_vulnerability->get_service(),

                /* Behavior: */
                service_vulnerability->get_service_behavior()
            );
        } else if (auto group_vulnerability = dynamic_cast<Group*>(vulnerability.get())) {
            apply_group(
                /* User: */
                group_vulnerability->get_user(),

                /* Group: */
                group_vulnerability->get_group(),

                /* Behavior: */
                group_vulnerability->get_group_behavior()
            );
        } else if (auto user_vulnerability = dynamic_cast<User*>(vulnerability.get())) {
            apply_user(
                /* User: */
                user_vulnerability->get_user(),

                /* Behavior: */
                user_vulnerability->get_user_behavior()
            );
        } else if (auto configuration_vulnerability = dynamic_cast<Configuration*>(vulnerability.get())) {
            // Compiler:
            (void) configuration_vulnerability;

            // Log:
            std::cout << "[!] Currently there is no support for configuration vulnerabilities in the applicator! Please configure them yourself!" << std::endl;
        } else if (auto permission_vulnerability = dynamic_cast<Permission*>(vulnerability.get())) {
			apply_permission(
				/* Path: */
				permission_vulnerability->get_path(),

				/* Permissions: */
				permission_vulnerability->get_permissions()
			);
		} else if (auto file_vulnerability = dynamic_cast<File*>(vulnerability.get())) {
			// Compiler:
            (void) file_vulnerability;

            // Log:
            std::cout << "[!] Currently there is no support for file vulnerabilities in the applicator! Please configure them yourself!" << std::endl;		
		}
    }
}


// Implementations:
void Applicator::construct_instructions(const std::string &image_title) {
    // Variables (Assignment):
    // HTML:
    std::ostringstream HTML_stream;

    // Label:
    std::string operating_system_label = generate_operating_system_label();

    // Content:
    std::string header = read_text_file("./assets/readme_header.txt");
    std::string sequel = read_text_file("./assets/readme_sequel.txt");
    std::string footer = read_text_file("./assets/readme_footer.txt");

    std::string custom = read_text_file("./configure-readme.html");

    // Marker:
    const std::string marker = "<!-- CONTENT_START -->";

    // Position:
    std::size_t insert_position = header.find(marker);

    if (insert_position == std::string::npos) {
        throw std::runtime_error("[!] CONTENT_START marker not found in header!");
    }

    // Injection:
    HTML_stream << header.substr(0, insert_position + marker.size()) << "\n";

    // Logic:
    HTML_stream << "<h1><b>" + image_title + " README </b></h1>";
    HTML_stream << sequel;
    HTML_stream << "<h2><b>" + operating_system_label + "</b></h2>";
    HTML_stream << "<p>It is company policy to use only use " + operating_system_label + " on this computer. It is also company policy to use only the latest, official, stable " + operating_system_label + " packages available for required software and services on this computer. Management has decided that the default web browser for all users on this computer should be the latest stable version of Firefox. ";
    HTML_stream << "Company policy is to never let users log in as root. If administrators need to run commands as root, they are required to use the \"sudo\" command.</p>";

    // Injection:
    HTML_stream << "<h2><b> README </b></h2>";
    HTML_stream << custom;

    // Footer:
    HTML_stream << footer;

    // File:
    std::ofstream instruction_file("README.html");

    if (!instruction_file.is_open()) {
        throw std::runtime_error("[!] Failed to open README file for writing!");
    }

    // Logic:
    instruction_file << HTML_stream.str();
    instruction_file.close();
}

void Applicator::apply(std::vector<std::unique_ptr<Vulnerability>> &&vulnerability_vector, std::vector<std::unique_ptr<Vulnerability>> &&penalty_vector) {
    // Vulnerabilities:
    applicator_logic(std::move(vulnerability_vector));

    // Penalties: 
    applicator_logic(std::move(penalty_vector));
}

void Applicator::configure_image(const std::string &main_user) {
    if (file_exists("/usr/share/lightdm") == true) {
        // Variables (Assignment):
        // Command:
        std::string command = "echo \"autologin-user=" + main_user + "\" >> /usr/share/lightdm/lightdm.conf.d/50-ubuntu.conf";

        // Logic:
        if (system(command.c_str()) != 0) {
            std::cout << "[!] (LightDM) Failed to configure autologin for user " + main_user + "!" << std::endl;
        } else {
            std::cout << "[+] Configured autologin in LightDM for user " + main_user + "!" << std::endl;
        }
    } else if (file_exists("/etc/gdm3") == true) {
        // Variables (Assignment):
        // Command:
        std::string command = "echo -e \"AutomaticLoginEnable=True\\nAutomaticLogin=" + main_user + "\" >> /etc/gdm3/daemon.conf";

        // Logic:
        if (system(command.c_str()) != 0) {
            std::cout << "[!] (GDM) Failed to configure autologin for user " + main_user + "!" << std::endl;
        } else {
            std::cout << "[+] Configured autologin in GDM for user " + main_user + "!" << std::endl;
        }
    } else {
        std::cout << "[-] Please configure autologin yourself!" << std::endl;
    }
}
