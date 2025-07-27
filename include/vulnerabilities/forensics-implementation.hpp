// Written by: Christopher Gholmieh
// Guards:
#ifndef __FORENSICS_IMPLEMENTATION_HPP__
#define __FORENSICS_IMPLEMENTATION_HPP__

// Headers:
// Implementations:
#include "vulnerability-implementation.hpp"

// Core:
#include <string>


// Forensics:
class Forensics : public Vulnerability {
    public:
        /* Constructor: */
        Forensics(const std::string &path, const std::string &question, const std::string &answer, const int points, const std::string &description);

        /* Evaluate: */
        void evaluate() override;

        /* Question: */
        std::string get_question() const {
            return this->question;
        }

        /* Path: */
        std::string get_path() const {
            return this->path;
        }

    private:
        /* Question: */
        const std::string question;

        /* Path: */
        const std::string path;

        /* Answer: */
        const std::string answer;
};

// Guard:
#endif /** __FORENSICS_IMPLEMENTATION_HPP__ */