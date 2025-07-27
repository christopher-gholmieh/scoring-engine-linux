// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerability-implementation.hpp"
#include "engine-implementation.hpp"

#include "scoreboard-implementation.hpp"

// Core:
#include <memory>
#include <vector>
#include <chrono>
#include <thread>

#include <iostream>


// Implementations:
Engine::Engine(std::vector<std::unique_ptr<Vulnerability>> &&vulnerability_vector, std::vector<std::unique_ptr<Vulnerability>> &&penalty_vector, const std::string& image_title)
    : vulnerability_vector(std::move(vulnerability_vector)), penalty_vector(std::move(penalty_vector)), image_title(image_title) {}

void Engine::activate() {
    // Variables (Assignment):
    // Points:
    int total_points = 0;

    for (std::unique_ptr<Vulnerability> &vulnerability: this->vulnerability_vector) {
        total_points += vulnerability->get_points();
    }

    // Scoreboard:
    Scoreboard scoreboard = {SCOREBOARD_PORT,  (int) this->vulnerability_vector.size(), total_points, image_title};

    // Scoreboard:
    std::thread scoreboard_thread([&] {
        scoreboard.enable();
    });

    // Engine:
    while (true) {
        // Variables (Assignment):
        // Remediations:
        std::vector<std::string> remediations = {};

        // Penalties:
        std::vector<std::string> penalties = {};

        // Points:
        int penalty_points = 0;
        int points = 0;

        for (const std::unique_ptr<Vulnerability>& vulnerability: this->vulnerability_vector) {
            // Evaluate:
            vulnerability->evaluate();

            // Logic:
            if (vulnerability->get_remediated() == true) {
                // Description:
                remediations.push_back(vulnerability->get_description());

                // Points:
                points += vulnerability->get_points();
            }
        }

        for (const std::unique_ptr<Vulnerability>& penalty: this->penalty_vector) {
            // Evaluate:
            penalty->evaluate();

            // Logic:
            if (penalty->get_remediated() == true) {
                // Description:
                penalties.push_back(penalty->get_description());

                // Points:
                penalty_points += penalty->get_points();
                points -= penalty->get_points();
            }
        }

        // Update:
        scoreboard.update(remediations, penalties, points, penalty_points);

        // Interval:
        std::this_thread::sleep_for(
            std::chrono::seconds(ENGINE_INTERVAL)
        );
    }
}