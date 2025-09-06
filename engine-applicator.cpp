// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "configuration-implementation.hpp"
#include "cryptography-implementation.hpp"
#include "cryptography-constants.hpp"

#include "applicator-implementation.hpp"
#include "engine-implementation.hpp"

// Core:
#include <iostream>
#include <unistd.h>


// Functions:
static std::string generate_operating_system_title() {
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

static void prepare_image(const std::string &main_user) {
    // Variables (Assignment):
    // Commands:
    const static std::vector<std::string> preparation_commands = {
        "mkdir -p /opt/scoring-engine/assets/",
        "strip ./build/scoring-engine",
        "cp ./build/scoring-engine /opt/scoring-engine/scoring-engine-binary",
        "cp ./configuration.dat /opt/scoring-engine/",
        "mv ./README.html /opt/scoring-engine/README.html",
        "chmod 444 /opt/scoring-engine/README.html",
        "cp ./assets/background.png /opt/scoring-engine/assets/",
        "cp ./assets/*.wav /opt/scoring-engine/assets/",
        "cp -r ./website/ /opt/scoring-engine/",
        "chown -R root:root /opt/scoring-engine/",
        "snap remove firefox",
        "rm -f /opt/firefox-*.tar.xz",
        "wget -P /opt/ --content-disposition \"https://download.mozilla.org/?product=firefox-latest&os=linux64&lang=en-US\"",
        "tar -xJf /opt/firefox-*.tar.xz -C /opt/",
        "ln -sf /opt/firefox/firefox /usr/bin/firefox",
        "rm -f /opt/firefox-*.tar.xz"
    };

    // Desktop:
    const std::string desktop_command = "cp ./assets/development/*.desktop /home/" + main_user + "/Desktop/ && chown " + main_user + ":" + main_user + " /home/" + main_user + "/Desktop/*.desktop";

    // Logic:
    for (const std::string &command : preparation_commands) {
        if (system(command.c_str()) != 0) {
            std::cout << "[!] Command: " + command + " failed!" << std::endl;
        }
    }

    if (system(desktop_command.c_str()) != 0) {
        std::cout << "[!] Failed to replicate desktop files to " + main_user + "!";
    } else {
        // Variables (Assignment):
        // Permissions:
        std::string permission_command = "chmod +x /home/" + main_user + "/Desktop/*.desktop";

        // Logic:
        if (system(permission_command.c_str()) == 0) {
            std::cout << "[*] Please make the desktop files launchable!" << std::endl;
        }
    }

    if (system("echo '@reboot root cd /opt/scoring-engine/ && ./scoring-engine-binary >> /var/log/scoring-engine.log 2>&1' >> /etc/crontab") != 0) {
        std::cout << "[!] Failed to add crontab to run scoring engine on system startup!" << std::endl;
    }

    std::cout << "[*] Please delete this entire folder if everything ran successfully! Everything has been copied to /opt/scoring-engine/" << std::endl;
}

static void clean_image() {
    // Variables (Assignment):
    // Responses:
    std::vector<std::string> responses = {
        "yes", "YES", "ye", "YE", "y", "Y"
    };

    // Response:
    std::string response;

    std::cout << "[*] Would you like remove all caches, log files, and overwrite timestamps, and potentially remove other forensics data from this machine? [y/N]: ";
    std::getline(std::cin, response);

    // Flag:
    bool flag = false;

    for (const std::string &value : responses) {
        if (response == value) {
            flag = true;

            break;
        }
    }

    if (flag == false) {
        std::cout << "[-] User declined to clean image." << std::endl;

        return;
    }

    // Paths:
    const std::string paths = "/bin /etc /home /opt /root /sbin /srv /usr /mnt /var";

    // Commands:
    const static std::vector<std::string> clean_commands = {
        "find " + paths + " -iname '*.viminfo*' -delete -o -iname '*.swp' -delete",
        "find " + paths + " -iname '*.bash_history' -exec ln -sf /dev/null {} \\;",
        "find " + paths + " -name '.zsh_history' -exec ln -sf /dev/null {} \\;",
        "find " + paths + " -name '.mysql_history' -exec rm -f {} \\;",
        "rm -rf /root/.local /home/*/.local/",
        "rm -rf /root/.cache /home/*/.cache/",
        "rm -rf /root/*~ /home/*/Desktop/*~",
        "rm -f /var/VMwareDnD/* /var/crash/*.crash",
        "rm -rf /var/log/apt/* /var/log/dpkg.log",
        "rm -f /var/log/auth.log* /var/log/syslog*",
        "rm -f /var/log/installer/initial-status.gz",
        "apt-get install -y bleachbit",
        "bleachbit --clean firefox.url_history",
        "bleachbit --clean firefox.cache",
        "find /etc /home /var -exec touch --date='2012-12-12 12:12' {} \\; 2>/dev/null",
    };

    // Logic:
    for (const std::string &clean_command: clean_commands) {
        if (system(clean_command.c_str()) != 0) {
            std::cout << "[!] Command failed: " + clean_command + "!" << std::endl;
        }
    }
}

// Main:
int main(void) {
    // Validation:
    if (geteuid() != 0) {
        std::cout << "[!] Please run this binary as root!" << std::endl;

        // Logic:
        return EXIT_FAILURE;
    }

    // Variables (Assignment):
    // Bytes:
    std::vector<unsigned char> file_bytes = read_file("configuration.dat");

    if (file_bytes.size() < NONCE_LENGTH) {
        throw std::runtime_error("[!] Configuration file is too short!");
    }

    // Nonce:
    std::vector<unsigned char> nonce(file_bytes.begin(), file_bytes.begin() + NONCE_length);

    // Cipher:
    std::vector<unsigned char> cipher_text(file_bytes.begin() + NONCE_length, file_bytes.end());

    // Key:
    std::vector<unsigned char> key(AES_ENCRYPTION_KEY, AES_ENCRYPTION_KEY + AES_ENCRYPTION_KEY_length);

    // Bytes:
    std::vector<unsigned char> plaintext_bytes = decrypt_configuration(key, cipher_text, nonce);

    // Text:
    std::string plaintext(plaintext_bytes.begin(), plaintext_bytes.end());

    // Vulnerabilities:
    std::vector<std::unique_ptr<Vulnerability>> vulnerability_vector = parse_yaml_configuration_vulnerabilities(plaintext);

    // Penalties:
    std::vector<std::unique_ptr<Vulnerability>> penalty_vector = parse_yaml_configuration_penalties(plaintext);

    // Title:
    std::string image_title = obtain_image_title(plaintext);

    // User:
    const std::string main_user = obtain_main_user(plaintext);

    if (image_title == "Score Report") {
        image_title = generate_operating_system_title();
    }

    // Applicator:
    Applicator applicator;

    // Logic:
    applicator.construct_instructions(image_title);
    applicator.apply(std::move(vulnerability_vector), std::move(penalty_vector));
    applicator.configure_image(main_user);

    prepare_image(main_user);
    clean_image();

    return 0;
}
