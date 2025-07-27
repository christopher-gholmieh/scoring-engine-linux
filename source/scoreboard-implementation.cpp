// Written by: Christopher Gholmieh
// Headers:

// Implementations:
#include "scoreboard-implementation.hpp"

// Core:
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <fstream>
#include <sstream>


// Functions:
static std::string get_path(std::string line) {
    // Variables (Assignment):
    // Stream:
    std::istringstream line_stream {line};

    // Method:
    std::string method;

    // Path:
    std::string path;

    // Version:
    std::string version;

    // Logic:
    line_stream >> method >> path >> version;

    return path;
}

static bool string_ends_with(const std::string &string, const std::string &suffix) {
    // Validation:
    if (suffix.size() > string.size()) {
        return false;
    }

    // Logic:
    return std::equal(suffix.rbegin(), suffix.rend(), string.rbegin());
}


// Implementations:
Scoreboard::Scoreboard(const int port, const int number_vulnerabilities, const int total_points, const std::string &image_title)
    : port(port), number_vulnerabilities(number_vulnerabilities), total_points(total_points), image_title(image_title) {}

void Scoreboard::update(std::vector<std::string> remediations, std::vector<std::string> penalties, int points, int penalty_points) {
    // Remediations:
    this->remediations = remediations;

    // Penalties: 
    this->penalties = penalties;

    // Points:
    this->penalty_points = penalty_points;
    this->points = points;
}

void Scoreboard::enable() {
    // Variables (Assignment):
    // Socket:
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket == -1) {
        // Message:
        std::cout << "[!] Unable to initialize server socket!" << std::endl;

        // Logic:
        return;
    }

    // Address:
    sockaddr_in server_address = {};

    /* Address: */
    server_address.sin_addr.s_addr = INADDR_ANY;

    /* Family: */
    server_address.sin_family = AF_INET;

    /* Port: */
    server_address.sin_port = htons(this->port);

    if (bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) == -1) {
        // Message:
        std::cout << "[!] Failed to bind server socket!" << std::endl;

        // Logic:
        return;
    }

    // Logic:
    if (listen(server_socket, 5) == -1) {
        // Message:
        std::cout << "[!] Failed to listen for incoming connections!" << std::endl;

        // Logic:
        return;
    }

    while (true) {
        // Variables (Assignment):
        // Client:
        int client_socket = accept(server_socket, nullptr, nullptr);

        if (client_socket == -1) {
            // Message:
            std::cout << "[!] Failed to accept client socket!" << std::endl;
            
            // Logic:
            continue;
        }

        // Bufer:
        char buffer[1024];
        
        // Bytes:
        ssize_t bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytes == -1) {
            // Message:
            std::cout << "[!] Failed to read from client socket!" << std::endl;

            // Logic:
            continue;
        }

        // Terminator:
        buffer[bytes] = '\0';

        // Variables (Assignment):
        // Request:
        std::string request {buffer};

        // Stream:
        std::stringstream request_stream {request};

        // Line:
        std::string line;

        std::getline(request_stream, line, '\n');

        // Path:
        std::string path = get_path(line);

        if (path == "/") {
            path = "/index.html";
        }

        // File:
        std::string file_path = "website" + path;

        // Stream:
        std::ifstream file_stream {file_path, std::ios::binary};

        // Logic:
        if (file_stream) {
            // Variables (Assignment):
            // Stream:
            std::ostringstream output_stream;

            output_stream << file_stream.rdbuf();

            // Content:
            std::string content = output_stream.str();

            // Type:
            std::string content_type = "text/html";

            if (string_ends_with(path, ".js") == true) {
                content_type = "application/javascript";
            } else if (string_ends_with(path, ".css") == true) {
                content_type = "text/css";
            }  else if (string_ends_with(path, ".png") == true) {
                content_type = "image/png";
            }

            // Response:
            std::string response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: " + content_type + "\r\n"
                "Content-Length: " + std::to_string(content.size()) + "\r\n\r\n" + content;

            // Logic:
            send(client_socket, response.c_str(), response.size(), 0);

            // Memory:
            close(client_socket);
        } else if (path == "/api") {
            // Variables (Assignment):
            // Content:
            std::string content = "{\"remediations\": [";

            // Remediations:
            for (size_t iterator = 0; iterator < this->remediations.size(); iterator++) {
                content += "\"" + this->remediations[iterator] + "\"";

                if (iterator != this->remediations.size() - 1) {
                    content += ",";
                }
            }

            content += "], ";

            // Penalties:
            content += "\"penalties\": [";

            for (size_t iterator = 0; iterator < this->penalties.size(); iterator++) {
                content += "\"" + this->penalties[iterator] + "\"";

                if (iterator != this->penalties.size() - 1) {
                    content += ",";
                }
            }

            content += "], ";

            // Points:
            content += "\"points\":" + std::to_string(this->points) + ", \"penalty_points\": " + std::to_string(this->penalty_points) + ", ";
            content += "\"total_points\": " + std::to_string(this->total_points) + ", ";

            content += "\"number_vulnerabilities\": " + std::to_string(this->number_vulnerabilities);

            // Title:
            content += ", \"image_title\": \"" + this->image_title + "\"}";

            // Response:
            std::string response = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type:  application/json\r\n"
                "Content-Length: " + std::to_string(content.size()) + "\r\n\r\n" + content;

            // Logic:
            send(client_socket, response.c_str(), response.size(), 0);

            // Memory:
            close(client_socket);
        }
    }
}