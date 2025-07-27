// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/user-implementation.hpp"

// Core:
#include <string>
#include <pwd.h>


// Functions:
static bool user_exists(const std::string &name) {
    // Variables (Assignment):
    // Entry:
    struct passwd* entry = getpwnam(name.c_str());

    // Logic:
    if (entry == nullptr) {
        return false;
    }

    return true;
}


// Implementations:
User::User(const std::string &name, const user_behavior_t user_behavior, const int points, const std::string &description)
    : Vulnerability(points, description), name(name), user_behavior(user_behavior) {}

void User::evaluate() {
    // Variables (Assignment):
    // Exists:
    const bool exists = user_exists(this->name);

    // Logic:
    switch (this->user_behavior) {
        case DELETE_USER: {
            if (exists == false && this->remediated == false) {
                this->remediated = true;

                break;
            }

            if (exists == true && this->remediated == true) {
                this->remediated = false;
            }

            break;
        }

        case CREATE_USER: {
            if (exists == true && this->remediated == false) {
                this->remediated = true;

                break;
            }

            if (exists == false && this->remediated == true) {
                this->remediated = false;
            }

            break;
        }
    }

}