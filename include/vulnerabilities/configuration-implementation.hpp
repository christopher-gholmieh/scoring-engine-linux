// Written by: Christopher Gholmieh
// Guards:
#ifndef __CONFIGURATION_IMPLEMENTATION_HPP__
#define __CONFIGURATION_IMPLEMENTATION_HPP__

// Headers:
// Implementations:
#include "vulnerability-implementation.hpp"

// Core:
#include <string>


// Enumerations:
typedef enum {
    FILE_CONTAINS_NOT,
    FILE_CONTAINS,
} configuration_behavior_t;


// Configuration:
class Configuration : public Vulnerability {
    public:
        /* Constructor: */
        Configuration(const std::string &path, const std::string &text, const configuration_behavior_t configuration_behavior, const int points, const std::string &description);

        /* Evaluate: */
        void evaluate() override;

        /* Path: */
        std::string get_path() const {
            return this->path;
        }

        /* Text: */
        std::string get_text() const {
            return this->text;
        }

        /* Behavior: */
        configuration_behavior_t get_configuration_behavior() const {
            return this->configuration_behavior;
        }

    private:
        /* Path: */
        const std::string path;

        /* Text: */
        const std::string text;

        /* Behavior: */
        const configuration_behavior_t configuration_behavior;
};

// Guard:
#endif /** __CONFIGURATION_IMPLEMENTATION_HPP__ */