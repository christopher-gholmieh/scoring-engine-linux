// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/configuration-implementation.hpp"
#include "vulnerability-implementation.hpp"

#include "filesystem-utilities.hpp"

// Core:
#include <string>


// Implementations:
Configuration::Configuration(const std::string &path, const std::string &text, const configuration_behavior_t configuration_behavior, const int points, const std::string &description)
    : Vulnerability(points, description), path(path), text(text), configuration_behavior(configuration_behavior) {}

void Configuration::evaluate() {
    // Variables (Assignment):
    // Contains:
    const bool contains = file_contains(this->path, this->text);

    // Logic:
    switch (this->configuration_behavior) {
        case FILE_CONTAINS_NOT: {
            if (contains == false && this->remediated == false) {
                this->remediated = true;

                break;
            }

            if (contains == true && this->remediated == true) {
                this->remediated = false;
            }

            break;
        }

        case FILE_CONTAINS: {
            if (contains == true && this->remediated == false) {
                this->remediated = true;

                break;
            }

            if (contains == false && this->remediated == true) {
                this->remediated = false;
            }

            break;
        }
    }
}