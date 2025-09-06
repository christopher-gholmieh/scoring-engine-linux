// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "configuration-implementation.hpp"
#include "cryptography-implementation.hpp"
#include "cryptography-constants.hpp"

#include "engine-implementation.hpp"
#include "image-globals.hpp"

// Core:
#include <iostream>
#include <unistd.h>


// Main:
int main(void) {
    // Validation:
    if (geteuid() != 0) {
        std::cout << "[!] Please run this binary as root!" << std::endl;

        // Logic:
        return EXIT_FAILURE;
    }

    // Variables (Assignment):
    // Bytes:
    std::vector<unsigned char> file_bytes = read_file("configuration.dat");

    if (file_bytes.size() < NONCE_LENGTH) {
        throw std::runtime_error("[!] Configuration file is too short!");
    }

    // Nonce:
    std::vector<unsigned char> nonce(file_bytes.begin(), file_bytes.begin() + NONCE_length);

    // Cipher:
    std::vector<unsigned char> cipher_text(file_bytes.begin() + NONCE_length, file_bytes.end());

    // Key:
    std::vector<unsigned char> key(AES_ENCRYPTION_KEY, AES_ENCRYPTION_KEY + AES_ENCRYPTION_KEY_length);

    // Bytes:
    std::vector<unsigned char> plaintext_bytes = decrypt_configuration(key, cipher_text, nonce);

    // Text:
    std::string plaintext(plaintext_bytes.begin(), plaintext_bytes.end());

    // Vulnerabilities:
    std::vector<std::unique_ptr<Vulnerability>> vulnerability_vector = parse_yaml_configuration_vulnerabilities(plaintext);

    // Penalties:
    std::vector<std::unique_ptr<Vulnerability>> penalty_vector = parse_yaml_configuration_penalties(plaintext);

    // Title:
    const std::string image_title = obtain_image_title(plaintext);
    global_image_title = image_title;

    // User:
    const std::string image_user = obtain_main_user(plaintext);
    global_image_user = image_user;

    // Engine:
    Engine engine(std::move(vulnerability_vector), std::move(penalty_vector), image_title);

    // Logic:
    engine.activate();

    return 0;
}