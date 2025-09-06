// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/user-implementation.hpp"

// Utilities:
#include "notification-utilities.hpp"
#include "sound-utilities.hpp"

// Core:
#include <string>
#include <pwd.h>


// Functions:
static bool user_exists(const std::string &name) {
    return getpwnam(name.c_str()) != nullptr;
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
                // Remediation:
                this->remediated = true;

                // Notification:
                construct_positive_notification();
                play_gain_sound();

                // Logic:
                break;
            }

            if (exists == true && this->remediated == true) {
                // Remediation:
                this->remediated = false;

                // Notification:
                construct_negative_notification();
                play_alarm_sound();
            }

            break;
        }

        case CREATE_USER: {
            if (exists == true && this->remediated == false) {
                // Remediation:
                this->remediated = true;

                // Notification:
                construct_positive_notification();
                play_gain_sound();

                // Logic:
                break;
            }

            if (exists == false && this->remediated == true) {
                // Remediation:
                this->remediated = false;

                // Notification:
                construct_negative_notification();
                play_alarm_sound();
            }

            break;
        }
    }
}