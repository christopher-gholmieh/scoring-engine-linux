// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/package-implementation.hpp"
#include "vulnerability-implementation.hpp"

// Core:
#include <cstdio>
#include <string>


// Functions:
static std::string get_package_version(const std::string &name) {
    // Variables (Assignment):
    // Command:
    const std::string command = "dpkg -s " + name + " 2>/dev/null | grep ^Version | cut -d' ' -f2";

    // Pipe:
    FILE* pipe = popen(command.c_str(), "r");

    if (pipe == nullptr) {
        return "";
    }

    // Version:
    std::string version;

    // Buffer:
    char buffer[128];

    if (fgets(buffer, sizeof(buffer), pipe)) {
        version = buffer;

        if (!version.empty() && version.back() == '\n') {
            version.pop_back();
        }
    }

    // Memory:
    pclose(pipe);

    // Logic:
    return version;
}

static bool version_greater_than(const std::string &version, const std::string &baseline) {
    // Variables (Assignment):
    // Command:
    const std::string command = "dpkg --compare-versions " + version + " gt " + baseline;

    // Logic:
    if (system(command.c_str()) == 0) {
        return true;
    } else {
        return false;
    }
}

static bool is_package_installed(const std::string &name) {
    // Variables (Assignment):
    // Command:
    const std::string command = "dpkg -s " + name + " 2>/dev/null | grep -q '^Status: install ok installed'";

    // Logic:
    if (system(command.c_str()) == 0) {
        return true;
    }

    return false;
}


// Implementations:
Package::Package(const std::string &name, const package_behavior_t package_behavior, const int points, const std::string &description)
    : Vulnerability(points, description), name(name), package_behavior(package_behavior) {
        if (this->package_behavior == PACKAGE_UPDATED) {
            this->package_version = get_package_version(name);
        }
    }

void Package::evaluate() {
    // Variables (Assignment):
    // Installed:
    const bool package_installed = is_package_installed(this->name);

    // Logic:
    switch (this->package_behavior) {
        case PACKAGE_INSTALLED: {
            if (package_installed == true && this->remediated == false) {
                this->remediated = true;

                break;
            }

            if (package_installed == false && this->remediated == true) {
                this->remediated = false;
            }

            break;
        }

        case PACKAGE_REMOVED: {
            if (package_installed == false && this->remediated == false) {
                this->remediated = true;

                break;
            }

            if (package_installed == true && this->remediated == true) {
                this->remediated = false;
            }

            break;
        }

        case PACKAGE_UPDATED: {
            // Variables (Assignment):
            // Version:
            const std::string current_version = get_package_version(this->name);

            if (current_version.empty()) {
                this->remediated = false;

                break;
            }

            // Updated:
            const bool updated = version_greater_than(current_version, this->package_version);

            // Logic:
            if (updated == true && this->remediated == false) {
                this->remediated = true;

                break;
            }

            if (updated == false && this->remediated == true) {
                this->remediated = false;
            }

            break;
        }
    }

}