// Written by: Christopher Gholmieh
// Guards:
#ifndef __FILESYSTEM_UTILITIES_H__
#define __FILESYSTEM_UTILITIES_H__

// Headers:
// Core:
#include <filesystem>
#include <fstream>
#include <string>


// Prototypes:
static inline bool file_contains(const std::string &path, const std::string &target);
static inline bool file_exists(const std::string &path);

static inline std::string file_permission(const std::string &path);


// Implementations:
static inline bool file_contains(const std::string &path, const std::string &target) {
    // Variables (Assignment):
    // Exists:
    bool exists = file_exists(path);

    if (exists == false) {
        return false;
    }

    // Stream:
    std::ifstream stream {path};

    if (stream.is_open() == false) {
        return false;
    }

    // Buffer:
    std::string buffer;

    // Logic:
    while (std::getline(stream, buffer)) {
        if (buffer.find(target) != std::string::npos) {
            return true;
        }
    }

    return false;
}

static inline bool file_exists(const std::string &path) {
    return std::filesystem::exists(path);
}

static inline std::string file_permission(const std::string &path) {
    // Validation:
    if (file_exists(path) == false) {
        return "-1";
    }

    // Variables (Assignment):
    // Status:
    std::filesystem::file_status file_status = std::filesystem::status(path);

    // Permissions:
    std::filesystem::perms permissions = file_status.permissions();

    // Bits:
    unsigned int permission_bits = static_cast<unsigned int>(permissions) & 0777;

    // Buffer:
    char buffer[5];

    std::snprintf(buffer, sizeof(buffer), "%04o", permission_bits);

    // Logic:
    return std::string {buffer};
}

// Guard:
#endif /** __FILESYSTEM_UTILITIES_H__ */