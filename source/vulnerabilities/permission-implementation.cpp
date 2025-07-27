// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/permission-implementation.hpp"
#include "vulnerability-implementation.hpp"

#include "filesystem-utilities.hpp"

// Core:
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
    const std::string permission = file_permission(this->path);

    // Match:
    const bool match = (permissions == this->permissions);

    // Logic:
    if (match == false && this->remediated == false) {
        this->remediated = true;

        return;
    }

    if (match == true && this->remediated == true) {
        this->remediated = false;
    }
}