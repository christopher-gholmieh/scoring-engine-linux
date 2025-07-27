// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/forensics-implementation.hpp"

// Utilities:
#include "filesystem-utilities.hpp"

// Core:
#include <string>


// Implementations:
Forensics::Forensics(const std::string &path, const std::string &question, const std::string &answer, const int points, const std::string &description)
    : Vulnerability(points, description), question(question), path(path), answer(answer) {}

void Forensics::evaluate() {
    // Variables (Assignment):
    // Exists:
    bool exists = file_exists(this->path);

    // Logic:
    if (this->penalized == false && exists == false) {
        this->remediated = false;
        this->penalized = true;

        return;
    }

    if (this->penalized == true && exists == true) {
        this->penalized = false;

        return;
    }

    this->remediated = file_contains(this->path, "ANSWER: " + this->answer);
}