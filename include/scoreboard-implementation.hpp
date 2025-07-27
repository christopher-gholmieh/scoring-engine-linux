// Written by: Christopher Gholmieh
// Guards:
#ifndef __SCOREBOARD_IMPLEMENTATION_HPP__
#define __SCOREBOARD_IMPLEMENTATION_HPP__

// Headers:
// Core:
#include <vector>
#include <string>


// Scoreboard:
class Scoreboard {
    public:
        /* Constructor: */
        Scoreboard(const int port, const int number_vulnerabilities, const int total_points, const std::string& image_title);

        /* Update: */
        void update(std::vector<std::string> remediations, std::vector<std::string> penalties, int points, int penalty_points);

        /* Enable: */
        void enable();

    private:
        /* Remediations: */
        std::vector<std::string> remediations = {};

        /* Penalties: */
        std::vector<std::string> penalties = {};

        /* Points: */
        int penalty_points = 0;
        int points = 0;

        /* Port: */
        const int port;

        /* Vulnerabilities: */
        const int number_vulnerabilities;

        /* Points: */
        const int total_points;

        /* Title: */
        const std::string image_title;
};

// Guard:
#endif /** __SCOREBOARD_IMPLEMENTATION_HPP__ */