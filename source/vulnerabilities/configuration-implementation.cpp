// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/configuration-implementation.hpp"
#include "vulnerability-implementation.hpp"

// Utilities:
#include "notification-utilities.hpp"
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
                // Remediation:
                this->remediated = true;

                // Notification:
                construct_positive_notification();

                // Logic:
                break;
            }

            if (contains == true && this->remediated == true) {
                // Remediation:
                this->remediated = false;

                // Notification:
                construct_negative_notification();
            }

            break;
        }

        case FILE_CONTAINS: {
            if (contains == true && this->remediated == false) {
                // Remediation:
                this->remediated = true;

                // Notification:
                construct_positive_notification();

                // Logic:
                break;
            }

            if (contains == false && this->remediated == true) {
                // Remediation:
                this->remediated = false;

                // Notification:
                construct_negative_notification();
            }

            break;
        }
    }
}