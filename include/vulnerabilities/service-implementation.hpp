// Written by: Christopher Gholmieh
// Guards:
#ifndef __SERVICE_IMPLEMENTATION_HPP__
#define __SERVICE_IMPLEMENTATION_HPP__

// Headers:
// Implementations:
#include "vulnerability-implementation.hpp"

// Core:
#include <string>


// Enumerations:
typedef enum {
    DISABLE_SERVICE,
    ENABLE_SERVICE,
} service_behavior_t;


// Service:
class Service : public Vulnerability {
    public:
        /* Constructor: */
        Service(const std::string &name, const service_behavior_t service_behavior, const int points, const std::string &description);

        /* Evaluate: */
        void evaluate() override;

        /* Service: */
        std::string get_service() const {
            return this->name;
        }

        /* Behavior: */
        service_behavior_t get_service_behavior() const {
            return this->service_behavior;
        }

    private:
        /* Name: */
        const std::string name;

        /* Behavior: */
        const service_behavior_t service_behavior;
};

// Guard:
#endif /** __SERVICE_IMPLEMENTATION_HPP__ */