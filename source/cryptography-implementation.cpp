// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "cryptography-implementation.hpp"

// Core:
#include <fstream>
#include <memory>


// Implementations:
std::vector<unsigned char> decrypt_configuration(
    const std::vector<unsigned char>& AES_encryption_key,
    const std::vector<unsigned char>& cipher_text,
    const std::vector<unsigned char>& nonce
) {
    // Validation:
    if (AES_encryption_key.size() != AES_KEY_SIZE) {
        throw OpenSSLException("[!] Invalid AES key size!");
    }

    if (cipher_text.size() < GCM_TAG_SIZE) {
        throw OpenSSLException("[!] Cipher text too short!");
    }

    if (nonce.size() != NONCE_LENGTH) {
        throw OpenSSLException("[!] Invalid nonce size!");
    }

    // Variables (Assignment):
    // Context:
    std::unique_ptr<EVP_CIPHER_CTX, EVP_Context_Deleter> context(EVP_CIPHER_CTX_new());

    if (context == nullptr) {
        throw OpenSSLException("[!] Unable to create EVP context!");
    }

    // Logic:
    if (EVP_DecryptInit_ex(context.get(), EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1) {
        throw OpenSSLException("[!] Failed to initialize decryption algorithm!");
    }

    if (EVP_CIPHER_CTX_ctrl(context.get(), EVP_CTRL_GCM_SET_IVLEN, NONCE_LENGTH, nullptr) != 1) {
        throw OpenSSLException("[!] Failed to set IV length!");
    }

    if (EVP_DecryptInit_ex(context.get(), nullptr, nullptr, AES_encryption_key.data(), nonce.data()) != 1) {
        throw OpenSSLException("[!] Failed to set AES encryption key and IV!");
    }

    // Variables (Assignment):
    // Cipher:
    int cipher_text_length = static_cast<int>(cipher_text.size());

    // Encrypt:
    int encrypted_data_length = cipher_text_length - GCM_TAG_SIZE;

    // Length:
    int length = 0;

    // Tag:
    const unsigned char* GCM_tag = cipher_text.data() + encrypted_data_length;

    // Text:
    std::vector<unsigned char> plaintext(encrypted_data_length);

    // Logic:
    if (EVP_DecryptUpdate(context.get(), plaintext.data(), &length, cipher_text.data(), encrypted_data_length) != 1) {
        throw OpenSSLException("[!] Failed to update decryption algorithm!");
    }

    if (EVP_CIPHER_CTX_ctrl(context.get(), EVP_CTRL_GCM_SET_TAG, GCM_TAG_SIZE, const_cast<unsigned char*> (GCM_tag)) != 1) {
        throw OpenSSLException("[!] Failed to set GCM tag!");
    }

    // Variables (Assignment):
    // Length:
    int final_length = 0;

    // Logic:
    if (EVP_DecryptFinal_ex(context.get(), plaintext.data() + length, &final_length) != 1) {
        throw OpenSSLException("[!] Failed to decrypt data!");
    }

    // Text:
    plaintext.resize(final_length + length);

    // Logic:
    return plaintext;
}

std::vector<unsigned char> read_file(const std::string &path) {
    // Variables (Assignment):
    // Stream:
    std::ifstream stream(path, std::ios::binary);

    if (stream.is_open() == false) {
        throw std::runtime_error("[!] Failed to open file: " + path + "!");
    }

    // Logic:
    return std::vector<unsigned char>(
        std::istreambuf_iterator<char>(stream),
        std::istreambuf_iterator<char>()
    );
}