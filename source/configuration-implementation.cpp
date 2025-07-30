// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "vulnerabilities/configuration-implementation.hpp"
#include "vulnerabilities/permission-implementation.hpp"
#include "vulnerabilities/forensics-implementation.hpp"
#include "vulnerabilities/package-implementation.hpp"
#include "vulnerabilities/service-implementation.hpp"
#include "vulnerabilities/group-implementation.hpp"
#include "vulnerabilities/user-implementation.hpp"
#include "vulnerabilities/file-implementation.hpp"

#include "vulnerability-implementation.hpp"
#include "configuration-implementation.hpp"

// Core:
#include <yaml-cpp/yaml.h>

#include <unordered_map>
#include <vector>
#include <string>


// Constants:
static const std::unordered_map<std::string, package_behavior_t> package_type_to_behavior = {
    {"PackageInstalled", PACKAGE_INSTALLED},
    {"PackageRemoved"  , PACKAGE_REMOVED},

    {"PackageUpdated"  , PACKAGE_UPDATED}
};

static const std::unordered_map<std::string, std::string> package_type_to_action = {
    {"PackageInstalled", "install"},
    {"PackageRemoved"  , "remove"},

    {"PackageUpdated"  , "update"}
};

static const std::unordered_map<std::string, service_behavior_t> service_type_to_behavior = {
    {"ServiceDisabled", DISABLE_SERVICE},
    {"ServiceEnabled" , ENABLE_SERVICE}   
};

static const std::unordered_map<std::string, std::string> service_type_to_action = {
    {"ServiceDisabled", "disabled"},
    {"ServiceEnabled" , "enabled"}
};

static const std::unordered_map<std::string, group_behavior_t> group_type_to_behavior = {
    {"UserInGroupNot", USER_IN_GROUP_NOT},
    {"UserInGroup"   , USER_IN_GROUP}   
};

static const std::unordered_map<std::string, user_behavior_t> user_type_to_behavior = {
    {"UserCreated", CREATE_USER},
    {"UserRemoved", DELETE_USER}
};

static const std::unordered_map<std::string, configuration_behavior_t> configuration_type_to_behavior = {
    {"FileContainsNot", FILE_CONTAINS_NOT},
    {"FileContains"   , FILE_CONTAINS}
};


// Functions:
static std::string combine_description(const std::string &description, const int points) {
    return description + " - " + std::to_string(points) + " pts";
}

static void forensics_construction_validation(const YAML::Node &node) {
    if (!node["number"]) {
        throw std::runtime_error("[!] Forensics node missing a number!");
    }

    if (!node["path"]) {
        throw std::runtime_error("[!] Forensics node missing a path!");
    }

    if (!node["question"]) {
        throw std::runtime_error("[!] Forensics node missing a question!");
    }

    if (!node["answer"]) {
        throw std::runtime_error("[!] Forensics node missing an answer!");
    }
}

static std::unique_ptr<Forensics> construct_forensics_from_node(const YAML::Node &node, const int points, bool autogenerate_description) {
    // Validation:
    forensics_construction_validation(node);

    // Variables (Assignment):
    // Description:
    std::string description = combine_description("Forensics #" + std::to_string(node["number"].as<int>()) + " solved", points);

    if (autogenerate_description == false) {
        description = combine_description(node["description"].as<std::string>(), points);
    }

    // Logic:
    return std::make_unique<Forensics>(
        /* Path: */
        node["path"].as<std::string>(),

        /* Question: */
        node["question"].as<std::string>(),

        /* Answer: */
        node["answer"].as<std::string>(),

        /* Points: */
        points,

        /* Description: */
        description
    );
}

static void package_construction_validation(const YAML::Node &node) {
    if (!node["package"]) {
        throw std::runtime_error("[!] Package node missing a package!");
    }
}

static std::unique_ptr<Package> construct_package_from_node(const YAML::Node &node, const int points, const std::string& type, bool autogenerate_description) {
    // Validation:
    package_construction_validation(node);

    // Variables (Assignment):
    // Package:
    const std::string package = node["package"].as<std::string>();

    // Description:
    std::string description;

    if (autogenerate_description) {
        description = combine_description("Package " + package + " " + package_type_to_action.at(type), points);
    } else {
        description = combine_description(node["description"].as<std::string>(), points);
    }

    // Logic:
    return std::make_unique<Package>(
        /* Name: */
        package,

        /* Behavior: */
        package_type_to_behavior.at(type),

        /* Points: */
        points,

        /* Description: */
        description
    );
}

static void service_construction_validation(const YAML::Node &node) {
    if (!node["service"]) {
        throw std::runtime_error("[!] Service node missing a service!");
    }
}

static std::unique_ptr<Service> construct_service_from_node(const YAML::Node &node, const int points, const std::string& type, bool autogenerate_description) {
    // Validation:
    service_construction_validation(node);

    // Variables (Assignment):
    // Service:
    const std::string service = node["service"].as<std::string>();

    // Description:
    std::string description;

    if (autogenerate_description) {
        description = combine_description("Service " + service + " " + service_type_to_action.at(type), points);
    } else {
        description = combine_description(node["description"].as<std::string>(), points);
    }

    // Logic:
    return std::make_unique<Service>(
        /* Name: */
        service,

        /* Behavior: */
        service_type_to_behavior.at(type),

        /* Points: */
        points,

        /* Description: */
        description
    );
}

static void group_construction_validation(const YAML::Node &node) {
    if (!node["group"]) {
        throw std::runtime_error("[!] Group node missing a group!");
    }

    if (!node["user"]) {
        throw std::runtime_error("[!] Group node missing a user!");
    }
}

static std::unique_ptr<Group> construct_group_from_node(const YAML::Node &node, const int points, const std::string& type, bool autogenerate_description) {
    // Validation:
    group_construction_validation(node);

    // Variables (Assignment):
    // Group:
    const std::string group = node["group"].as<std::string>();

    // User:
    const std::string user = node["user"].as<std::string>();

    // Description:
    std::string description;

    if (autogenerate_description) {
        if (type == "UserInGroupNot") {
            description = combine_description("Removed " + user + " from group " + group, points);
        } else {
            description = combine_description("Added " + user + " to group " + group, points); 
        }
    } else {
        description = combine_description(node["description"].as<std::string>(), points);
    }

    // Logic:
    return std::make_unique<Group>(
        /* Name: */
        user,

        /* Group: */
        group,

        /* Behavior: */
        group_type_to_behavior.at(type),

        /* Points: */
        points,

        /* Description: */
        description
    );
}

static void user_construction_validation(const YAML::Node &node) {
    if (!node["user"]) {
        throw std::runtime_error("[!] User node missing a user!");
    }
}

static std::unique_ptr<User> construct_user_from_node(const YAML::Node &node, const int points, const std::string &type, bool autogenerate_description) {
    // Validation:
    user_construction_validation(node);

    // Variables (Assignment):
    // User:
    const std::string user = node["user"].as<std::string>();

    // Description:
    std::string description;

    if (autogenerate_description) {
        description = combine_description(std::string(type == "UserCreated" ? "Created" : "Removed") + " " + user, points);
    } else {
        description = combine_description(node["description"].as<std::string>(), points);
    }

    // Logic:
    return std::make_unique<User>(
        /* Name: */
        user,

        /* Behavior: */
        user_type_to_behavior.at(type),

        /* Points: */
        points,

        /* Description: */
        description
    );
}

static void configuration_construction_validation(const YAML::Node &node) {
    if (!node["path"]) {
        throw std::runtime_error("[!] Configuration node missing a path!");
    } 

    if (!node["text"]) {
        throw std::runtime_error("[!] Configuration node missing text!");
    }
}

static std::unique_ptr<Configuration> construct_configuration_from_node(const YAML::Node &node, const int points, const std::string &type, bool autogenerate_description) {
    // Validation:
    if (autogenerate_description) {
        throw std::runtime_error("[!] Unable to autogenerate a description for configuration nodes. Please write your own!");
    }

    configuration_construction_validation(node);

    // Variables (Assignment):
    // Behavior:
    configuration_behavior_t configuration_behavior = configuration_type_to_behavior.at(type);

    // Path:
    std::string path = node["path"].as<std::string>();

    // Text:
    std::string text = node["text"].as<std::string>();

    // Description:
    std::string description = combine_description(node["description"].as<std::string>(), points);

    // Logic:
    return std::make_unique<Configuration>(
        /* Path: */
        path,

        /* Text: */
        text,

        /* Behavior: */
        configuration_behavior,

        /* Points: */
        points,

        /* Description: */
        description
    );
}

static void permission_construction_validation(const YAML::Node &node) {
	if (!node["permission"]) {
		throw std::runtime_error("[!] Permission node missing a permission!");
	}

	if (!node["path"]) {
		throw std::runtime_error("[!] Permission node missing a path!");
	}
}

static std::unique_ptr<Permission> construct_permission_from_node(const YAML::Node &node, const int points, bool autogenerate_description) {
	// Validation:
	permission_construction_validation(node);

	// Variables (Assignment):
	// Permission:
	std::string permission = node["permission"].as<std::string>();

	// Path:
	std::string path = node["path"].as<std::string>();

	// Description:
	std::string description;

	if (autogenerate_description) {
		description = combine_description("Permissions on " + path + " fixed", points);
	} else {
		description = combine_description(node["description"].as<std::string>(), points);
	}

	// Logic:
	return std::make_unique<Permission>(
		/* Path: */
		path,

		/* Description: */
        permission,

		/* Points: */
		points,

		/* Description: */
		description
	);
}

static void file_construction_validation(const YAML::Node &node) {
	if (!node["path"]) {
		throw std::runtime_error("[!] File node missing a path!");
	}
}

static std::unique_ptr<File> construct_file_from_node(const YAML::Node &node, const int points, bool autogenerate_description) {
	// Validation:
	file_construction_validation(node);

	// Variables (Assignment):
	// Path:
	std::string path = node["path"].as<std::string>();

	// Description:
	std::string description;

	if (autogenerate_description) {
		description = combine_description("Removed file " + path, points);
	} else {
		description = node["description"].as<std::string>();
	}

	// Logic:
	return std::make_unique<File>(
		/* Path: */
		path,

		/* Points: */
		points,

		/* Description: */
		description
	);
}


// Implementations:
std::unique_ptr<Vulnerability> create_vulnerability_from_yaml(const YAML::Node &node) {
    // Validation:
    if (!node["type"]) {
        throw std::runtime_error("[!] Node missing a type!");
    }

    if (!node["points"]) {
        throw std::runtime_error("[!] Node missing points!");
    }

    // Variables (Assignment):
    // Description:
    bool autogenerate_description = true;

    if (node["description"]) {
        autogenerate_description = false;
    }

    // Points:
    const int points = node["points"].as<int>();

    // Type:
    const std::string type = node["type"].as<std::string>();

    // Forensics:
    if (type == "Forensics") {
        return construct_forensics_from_node(node, points, autogenerate_description);
    } 

    // Package:
    if (type == "PackageInstalled" || type == "PackageRemoved" || type == "PackageUpdated") {
        return construct_package_from_node(node, points, type, autogenerate_description);
    }

    // Service:
    if (type == "ServiceEnabled" || type == "ServiceDisabled") {
        return construct_service_from_node(node, points, type, autogenerate_description);
    }

    // Group:
    if (type == "UserInGroupNot" || type == "UserInGroup") {
        return construct_group_from_node(node, points, type, autogenerate_description);
    }
    
    // User:
    if (type == "UserCreated" || type == "UserRemoved") {
        return construct_user_from_node(node, points, type, autogenerate_description);
    }

    // Configuration:
    if (type == "FileContainsNot" || type == "FileContains") {
        return construct_configuration_from_node(node, points, type, autogenerate_description);
    }

	// Permission:
	if (type == "PermissionNot") {
		return construct_permission_from_node(node, points, autogenerate_description);
	}

	// File:
	if (type == "FileExistsNot") {
		return construct_file_from_node(node, points, autogenerate_description);
	}

    // Error:
    throw std::runtime_error("[!] Type '" + type + "' is not a valid type! Check documentation for valid types!");
}

std::vector<std::unique_ptr<Vulnerability>> parse_yaml_configuration_vulnerabilities(const std::string &plaintext) {
    // Variables (Assignment):
    // Vulnerabilities:
    std::vector<std::unique_ptr<Vulnerability>> vulnerability_vector;

    // Root:
    YAML::Node root = YAML::Load(plaintext);

    if (!root["vulnerabilities"]) {
        throw std::runtime_error("[!] Missing vulnerabilities key in YAML configuration!");
    }

    // Logic:
    for (const auto &item : root["vulnerabilities"]) {
        // Variables (Assignment):
        // Node:
        const YAML::Node &vulnerability_node = item["vulnerability"];

        // Logic:
        vulnerability_vector.push_back(create_vulnerability_from_yaml(vulnerability_node));
    }

    return vulnerability_vector;
}

std::vector<std::unique_ptr<Vulnerability>> parse_yaml_configuration_penalties(const std::string &plaintext) {
    // Variables (Assignment):
    // Penalties:
    std::vector<std::unique_ptr<Vulnerability>> penalty_vector;

    // Root:
    YAML::Node root = YAML::Load(plaintext);

    if (!root["penalties"]) {
        return penalty_vector;
    }

    // Logic:
    for (const auto &item: root["penalties"]) {
        // Variables (Assignment):
        // Node:
        const YAML::Node &penalty_node = item["penalty"];

        // Logic:
        penalty_vector.push_back(create_vulnerability_from_yaml(penalty_node));
    }

    return penalty_vector;
}

std::string obtain_image_title(const std::string &plaintext) {
    // Variables (Assignment):
    // Root:
    YAML::Node root = YAML::Load(plaintext);

    // Logic:
    if (!root["image"]) {
        return std::string {"Score Report"};
    }

    // Image:
    const YAML::Node &image_node = root["image"];

    if (!image_node["title"]) {
        return std::string {"Score Report"};
    } else {
        return image_node["title"].as<std::string>();
    }
}

std::string obtain_main_user(const std::string &plaintext) {
    // Variables (Assignment):
    // Root:
    YAML::Node root = YAML::Load(plaintext);

    // Logic:
    if (!root["image"]) {
        throw std::runtime_error("[!] No main user found! Please define a main user!");
    }

    // Image:
    const YAML::Node &image_node = root["image"];

    if (!image_node["user"]) {
        throw std::runtime_error("[!] No main user found! Please define a main user!");
    } else {
        return image_node["user"].as<std::string>();
    }
}
