// Written by: Christopher Gholmieh
// Guards:
#ifndef __FILE_IMPLEMENTATION_HPP__
#define __FILE_IMPLEMENTATION_HPP__

// Headers:
// Implementations:
#include "vulnerability-implementation.hpp"

// Core:
#include <string>


// File:
class File : public Vulnerability {
	public:
		/* Constructor: */
		File(const std::string &path, const int points, const std::string &description);

		/* Evaluate: */
		void evaluate() override;

		/* Path: */
		std::string get_path() const {
			return this->path;
		}

	private:
		/* Path: */
		const std::string path;
};

// Guard:
#endif /** __FILE_IMPLEMENTATION_HPP */
