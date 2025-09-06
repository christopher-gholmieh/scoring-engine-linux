// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/file-implementation.hpp"
#include "vulnerability-implementation.hpp"

// Utilities:
#include "notification-utilities.hpp"
#include "filesystem-utilities.hpp"

// Core:
#include <string>


// Implementations:
File::File(const std::string &path, const int points, const std::string &description)
	: Vulnerability(points, description), path(path) {}

void File::evaluate() {
	// Variables (Assignment):
	// Exists:
	bool exists = file_exists(this->path);

	// Logic:
	if (exists == false && this->remediated == false) {
		// Remediation:
		this->remediated = true;

		// Notification:
		construct_positive_notification();

		// Logic:
		return;
	}

	if (exists == true && this->remediated == true) {
		// Remediation:
		this->remediated = false;

		// Notification:
		construct_positive_notification();
	}
}
