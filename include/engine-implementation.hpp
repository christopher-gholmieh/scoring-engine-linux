// Written by: Christopher Gholmieh
// Guards:
#ifndef __ENGINE_IMPLEMENTATION_HPP__
#define __ENGINE_IMPLEMENTATION_HPP__

// Constants:
constexpr int SCOREBOARD_PORT = 8080;
constexpr int ENGINE_INTERVAL = 5;

// Headers:
// Implementations:
#include "vulnerability-implementation.hpp"

// Core:
#include <memory>
#include <vector>


// Engine:
class Engine {
    public:
        /* Constructor: */
        Engine(std::vector<std::unique_ptr<Vulnerability>> &&vulnerability_vector, std::vector<std::unique_ptr<Vulnerability>> &&penalty_vector, const std::string &image_title);

        /* Activate: */
        void activate();

    private:
        /* Vulnerabilities:*/
        std::vector<std::unique_ptr<Vulnerability>> vulnerability_vector;

        /* Penalties: */
        std::vector<std::unique_ptr<Vulnerability>> penalty_vector;


        /* Title: */
        const std::string image_title;
};

// Guard:
#endif /** __ENGINE_IMPLEMENTATION_HPP__ */
