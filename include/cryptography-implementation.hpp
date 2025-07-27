// Written by: Christopher Gholmieh
// Guards:
#ifndef __CRYPTOGRAPHY_IMPLEMENTATION_HPP__
#define __CRYPTOGRAPHY_IMPLEMENTATION_HPP__

// Constants:
constexpr int NONCE_LENGTH = 12;
constexpr int AES_KEY_SIZE = 32;
constexpr int GCM_TAG_SIZE = 16;

// Headers:
// Core:
#include <openssl/evp.h>

#include <stdexcept>
#include <vector>
#include <string>


// Exception:
class OpenSSLException : public std::runtime_error {
    public:
        /* Constructor: */
        explicit OpenSSLException(const std::string &message)
            : std::runtime_error(message) {};
};

// Context:
struct EVP_Context_Deleter {
    /* Operator: */
    void operator() (EVP_CIPHER_CTX* context) const { 
        EVP_CIPHER_CTX_free(context); 
    }
};

// Prototypes:
std::vector<unsigned char> decrypt_configuration(
    const std::vector<unsigned char> &AES_encryption_key,
    const std::vector<unsigned char> &cipher_text,
    const std::vector<unsigned char> &nonce
);

// Guard:
#endif /** __CRYPTOGRAPHY_IMPLEMENTATION_HPP__ */