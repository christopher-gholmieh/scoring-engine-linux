// Written by: Christopher Gholmieh
// Guards:
#ifndef __PERMISSION_IMPLEMENTATION_HPP__
#define __PERMISSION_IMPLEMENTATION_HPP__

// Headers:
// Implementations:
#include "vulnerability-implementation.hpp"

// Core:
#include <string>


// Permission:
class Permission : public Vulnerability {
    public:
        /* Constructor: */
        Permission(const std::string &path, const std::string &permissions, const int points, const std::string &description);

        /* Evaluate: */
        void evaluate() override;

		/* Path: */
		std::string get_path() const {
			return this->path;
		}

		/* Permissions: */
		std::string get_permissions() const {
			return this->permissions;
		}

    private:
        /* Path: */
        const std::string path;

        /* Permission: */
        const std::string permissions;
};

// Guard:
#endif /** __PERMISSION_IMPLEMENTATION_HPP__ */
