// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/forensics-implementation.hpp"

// Utilities:
#include "notification-utilities.hpp"
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
        // Remediation:
        this->remediated = false;

		// Notification:
		construct_negative_notification();

        // Penalty:
        this->penalized = true;

        // Logic:
        return;
    }

    if (this->penalized == true && exists == true) {
        // Penalty:
        this->penalized = false;

		// Notification:
		construct_positive_notification();

        // Logic:
        return;
    }

    // Variables (Assignment):
    // Contains:
    bool contains = file_contains(this->path, "ANSWER: " + this->answer);

    // Logic:
    if (contains && !this->remediated) {
        // Remediation:
        this->remediated = true;

		// Notification:
		construct_positive_notification();
    }
}