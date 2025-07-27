// Written by: Christopher Gholmieh
// Guards:
#ifndef __PACKAGE_IMPLEMENTATION_HPP__
#define __PACKAGE_IMPLEMENTATION_HPP__

// Headers:
// Implementations:
#include "vulnerability-implementation.hpp"

// Core:
#include <string>


// Enumerations:
typedef enum {
    PACKAGE_INSTALLED,
    PACKAGE_REMOVED,

    PACKAGE_UPDATED,
} package_behavior_t;


// Package:
class Package : public Vulnerability {
    public:
        /* Constructor: */
        Package(const std::string &name, const package_behavior_t package_behavior, const int points, const std::string &description);

        /* Evaluate: */
        void evaluate() override;

        /* Behavior: */
        package_behavior_t get_package_behavior() const {
            return this->package_behavior;
        }

        /* Package: */
        std::string get_package() const {
            return this->name;
        }

    private:
        /* Name: */
        const std::string &name;

        /* Behavior: */
        const package_behavior_t package_behavior;

        /* Version: */
        std::string package_version;
};

// Guard:
#endif /** __PACKAGE_IMPLEMENTATION_HPP__ */