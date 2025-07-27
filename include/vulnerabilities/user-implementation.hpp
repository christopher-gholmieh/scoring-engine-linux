// Written by: Christopher Gholmieh
// Guards:
#ifndef __USER_IMPLEMENTATIONS_HPP__
#define __USER_IMPLEMENTATIONS_HPP__

// Headers:
// Implementations:
#include "vulnerability-implementation.hpp"

// Core:
#include <string>


// Enumerations:
typedef enum {
    DELETE_USER,
    CREATE_USER,
} user_behavior_t;


// User:
class User : public Vulnerability {
    public:
        /* Constructor: */
        User(const std::string &name, const user_behavior_t user_behavior, const int points, const std::string &description);

        /* Evaluate: */
        void evaluate() override;

        /* User: */
        std::string get_user() const {
            return this->name;
        }

        /* Behavior: */
        user_behavior_t get_user_behavior() const {
            return this->user_behavior;
        }

    private:
        /* Name: */
        const std::string name;

        /* Behavior: */
        const user_behavior_t user_behavior;
};

// Guard:
#endif /** __USER_IMPLEMENTATION_HPP__ */