// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/service-implementation.hpp"

// Core:
#include <iostream>
#include <string>


// Functions:
static bool is_service_enabled(const std::string &name) {
    // Variables (Assignment):
    // Command:
    const std::string command = "systemctl is-enabled " + name + " 2>/dev/null";

    // File:
    FILE* pipe = popen(command.c_str(), "r");

    if (pipe == nullptr) {
        return false;
    }

    // Result:
    std::string result;

    // Buffer:
    char buffer[128];

    // Logic:
    if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result = buffer;
    }

    // Memory:
    pclose(pipe);

    // Logic:
    if (result.empty() == false && result.back() == '\n') {
        result.pop_back();
    }

    if (result == "enabled") {
        return true;
    }

    return false;
}


// Implementations:
Service::Service(const std::string &name, const service_behavior_t service_behavior, const int points, const std::string &description)
    : Vulnerability(points, description), name(name), service_behavior(service_behavior) {}

void Service::evaluate() {
    // Variables (Assignment):
    // Enabled:
    bool enabled = is_service_enabled(this->name);

    // Logic:
    switch (this->service_behavior) {
        case DISABLE_SERVICE: {
            if (enabled == false && this->remediated == false) {
                this->remediated = true;

                break;
            }

            if (enabled == true && this->remediated == true) {
                this->remediated = false;
            }

            break;
        }

        case ENABLE_SERVICE: {
            if (enabled == true && this->remediated == false) {
                this->remediated = true;

                break;
            }

            if (enabled == false && this->remediated == true) {
                this->remediated = false;
            }

            break;
        }
    }
}