// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/group-implementation.hpp"
#include "vulnerability-implementation.hpp"

// Utilities:
#include "notification-utilities.hpp"
#include "sound-utilities.hpp"

// Core:
#include <string>
#include <pwd.h>
#include <grp.h>


// Functions:
static bool group_exists(const std::string &group) {
    return getgrnam(group.c_str()) != nullptr;
}

static bool user_in_group(const std::string &name, const std::string &group) {
    // Validation:
    if (group_exists(group) == false) {
        return false;
    }

    // Variables (Assignment):
    // Entry:
    const struct passwd* entry = getpwnam(name.c_str());

    if (entry == nullptr) {
        return false;
    }

    // Entry:
    const struct group* group_entry = getgrnam(group.c_str());

    // Groups:
    int number_groups = 128;
    gid_t groups[128];

    // Logic:
    if (getgrouplist(name.c_str(), entry->pw_gid, groups, &number_groups) == -1) {
        return false;
    }

    for (int iterator = 0; iterator < number_groups; iterator++) {
        if (groups[iterator] == group_entry->gr_gid) {
            return true;
        }
    }

    return false;
}


// Implementations:
Group::Group(const std::string &name, const std::string &group, const group_behavior_t group_behavior, const int points, const std::string &description)
    : Vulnerability(points, description), name(name), group(group), group_behavior(group_behavior) {}

void Group::evaluate() {
    // Variables (Assignment):
    // Group:
    const bool in_group = user_in_group(name, group);

    // Logic:
    switch (this->group_behavior) {
        case USER_IN_GROUP_NOT: {
            if (in_group == false && this->remediated == false) {
                // Remediation:
                this->remediated = true;

                // Notification:
                construct_positive_notification();
                play_gain_sound();
            } else if (in_group == true && this->remediated == true) {
                this->remediated = false;

                // Notification:
                construct_negative_notification();
                play_alarm_sound();
            }

            break;
        }

        case USER_IN_GROUP: {
            if (in_group == true && this->remediated == false) {
                // Remediation:
                this->remediated = true;

                // Notification:
                construct_positive_notification();
                play_gain_sound();
            } else if (in_group == false && this->remediated == true) {
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