// Written by: Christopher Gholmieh
// Guards:
#ifndef __NOTIFICATION_UTILITIES_HPP__
#define __NOTIFICATION_UTILITIES_HPP__

// Headers:
// Implementations:
#include "image-globals.hpp"

// Core:
#include <iostream>
#include <string>
#include <pwd.h>


// Prototypes:
static inline void construct_positive_notification(void);
static inline void construct_negative_notification(void);

static inline std::string get_user_address(std::string name);
static inline uid_t get_user_uid(std::string name);


// Implementations:
static inline void construct_positive_notification() {
    // Variables (Assignment):
    // Command:
    std::string command = 
        "sudo -u " + global_image_user + " DBUS_SESSION_BUS_ADDRESS=" + get_user_address(global_image_user) +
        " DISPLAY=:0 notify-send \"" + global_image_title + "\" \"" + "You have gained points!" + "\"";

    // Logic:
    if (system(command.c_str()) != 0) {
        std::cout << "[!] Failed to notify a positive notification!" << std::endl;
    }
}

static inline void construct_negative_notification(void) {
    // Variables (Assignment):
    // Command:
    std::string command = 
        "sudo -u " + global_image_user + " DBUS_SESSION_BUS_ADDRESS=" + get_user_address(global_image_user) +
        " DISPLAY=:0 notify-send \"" + global_image_title + "\" \"" + "You have lost points!" + "\"";

    // Logic:
    if (system(command.c_str()) != 0) {
        std::cout << "[!] Failed to notify a negative notification!" << std::endl;
    }
}

static inline std::string get_user_address(std::string name) {
    // Variables (Assignment):
    // UID:
    uid_t user_uid = get_user_uid(name);
    
    if (user_uid == 0) {
        return "";
    }

    // Logic:
    return "unix:path=/run/user/" + std::to_string(user_uid) + "/bus";
}

static inline uid_t get_user_uid(std::string name) {
    // Variables (Assignment):
    // Record:
    struct passwd* record = getpwnam(name.c_str());

    if (record == nullptr) {
        // Logging:
        std::cout << "[!] User not found: " << name << std::endl;

        // Logic:
        return 0;
    }

    // Logic:
    return record->pw_uid;
}

// Guard:
#endif /** __NOTIFICATION_UTILITIES_HPP__ */