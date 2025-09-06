// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/permission-implementation.hpp"
#include "vulnerability-implementation.hpp"

// Utilities:
#include "notification-utilities.hpp"
#include "filesystem-utilities.hpp"

// Core:
#include <iostream>
#include <string>


// Implementations:
Permission::Permission(const std::string &path, const std::string &permissions, const int points, const std::string &description)
    : Vulnerability(points, description), path(path), permissions(permissions) {}

void Permission::evaluate() {
    // Variables (Assignment):
    // Exists:
    const bool exists = file_exists(this->path);

    if (exists == false) {
        return;
    }

    // Permission:
    const std::string permission_bits = file_permission(this->path);

    // Match:
    const bool match = permission_bits == this->permissions;

    // Logic:
    if (match == false && this->remediated == false) {
        // Remediation:
        this->remediated = true;

        // Notification:
        construct_positive_notification();

        // Logic:
        return;
    }

    if (match == true && this->remediated == true) {
        // Remediation:
        this->remediated = false;

        // Notification:
        construct_negative_notification();
    }
}
