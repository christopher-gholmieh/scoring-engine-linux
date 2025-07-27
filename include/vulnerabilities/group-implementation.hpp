// Written by: Christopher Gholmieh
// Guards:
#ifndef __GROUP_IMPLEMENTATION_HPP__
#define __GROUP_IMPLEMENTATION_HPP__

// Headers:
// Implementations:
#include "vulnerability-implementation.hpp"

// Core:
#include <string>


// Enumerations:
typedef enum {
    USER_IN_GROUP_NOT,
    USER_IN_GROUP,
} group_behavior_t;


// Group:
class Group : public Vulnerability {
    public:
        /* Constructor: */
        Group(const std::string &name, const std::string &group, const group_behavior_t group_behavior, const int points, const std::string &description);

        /* Evaluate: */
        void evaluate() override;

        /* User: */
        std::string get_user() const {
            return this->name;
        }

        /* Group: */
        std::string get_group() const {
            return this->group;
        }

        /* Behavior: */
        group_behavior_t get_group_behavior() const {
            return this->group_behavior;
        }

    private:
        /* Name: */
        const std::string name;

        /* Group: */
        const std::string group;

        /* Behavior: */
        const group_behavior_t group_behavior;
};

// Guard:
#endif /** __GROUP_IMPLEMENTATION_HPP__ */