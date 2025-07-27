// Written by: Christopher Gholmieh
// Guards:
#ifndef __CONFIGURATION_IMPLEMENTATION_HPP__
#define __CONFIGURATION_IMPLEMENTATION_HPP__

// Headers:
// Implementations:
#include "vulnerability-implementation.hpp"

// Core:
#include <yaml-cpp/yaml.h>

#include <memory>
#include <vector>
#include <string>


// Prototypes:
std::unique_ptr<Vulnerability> create_vulnerability_from_yaml(const YAML::Node &node);

std::vector<std::unique_ptr<Vulnerability>> parse_yaml_configuration_vulnerabilities(const std::string &plaintext);
std::vector<std::unique_ptr<Vulnerability>> parse_yaml_configuration_penalties(const std::string &plaintext);

std::string obtain_image_title(const std::string &plaintext);
std::string obtain_main_user(const std::string &plaintext);

std::vector<unsigned char> read_file(const std::string &path);

// Guard:
#endif /** __CONFIGURATION_IMPLEMENTATION_HPP__ */