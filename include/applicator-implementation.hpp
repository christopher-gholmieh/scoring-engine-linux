// Written by: Christopher Gholmieh
// Guards:
#ifndef __APPLICATOR_IMPLEMENTATION_H__
#define __APPLICATOR_IMPLEMENTATION_H__

// Headers:
// Implementations:
#include "vulnerability-implementation.hpp"

// Core:
#include <memory>
#include <vector>


// Applicator:
class Applicator {
    public:
        /* Constructor: */
        Applicator() {}

        /* Instructions: */
        void construct_instructions(const std::string &image_title);

        /* Apply: */
        void apply(std::vector<std::unique_ptr<Vulnerability>> &&vulnerability_vector, std::vector<std::unique_ptr<Vulnerability>> &&penalty_vector);

        /* Configure: */
        void configure_image(const std::string &main_user);
    private:
};

// Guard:
#endif /** __APPLICATOR_IMPLEMENTATION_H__ */