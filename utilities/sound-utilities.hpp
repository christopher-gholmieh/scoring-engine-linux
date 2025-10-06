// Written by: Chrisotpher Gholmieh
// Guards:
#ifndef __SOUND_UTILITIES_HPP__
#define __SOUND_UTILITIES_HPP__

// Headers:
// Core:
#include <iostream>
#include <atomic>

#include <cstdlib>
#include <cstdio>

#include <pthread.h>
#include <unistd.h>


// Global:
static std::atomic<bool> playing_alarm(false);
static std::atomic<bool> playing_gain(false);

// Prototypes:
static inline void play_alarm_sound(void);
static inline void play_gain_sound(void);

// Helpers:
static inline double get_wav_duration(const char* filepath) {
    // Variables (Assignment):
    // Duration:
    double duration = 1.0;

    // Command:
    std::string command = "soxi -D " + std::string {filepath};

    // Pipe:
    FILE* pipe = popen(command.c_str(), "r");

    if (pipe == nullptr) {
        // Log:
        std::cout << "[!] Failed to open pipe for soxi!" << std::endl;

        // Logic:
        return duration;
    }

    if (fscanf(pipe, "%lf", &duration) != 1) {
        // Log:
        std::cout << "[!] Failed to parse soxi output!" << std::endl;

        // Logic:
        duration = 1.0;
    }

    // Memory:
    pclose(pipe);

    // Logic:
    return duration;
}

// Reset threads:
static inline void* reset_alarm_flag_thread(void* argument) {
    // Compiler:
    (void) argument;

    // Variables (Assignment):
    // Duration:
    double duration = get_wav_duration("./assets/alarm.wav");
    sleep((unsigned int)(duration + 0.5));

    // State:
    playing_alarm = false;

    // Logic:
    return nullptr;
}

static inline void* reset_gain_flag_thread(void* argument) {
    // Compiler:
    (void) argument;

    // Variables (Assignment):
    // Duration:
    double duration = get_wav_duration("./assets/gain.wav");
    sleep((unsigned int)(duration + 0.5));

    // State:
    playing_gain = false;

    // Logic:
    return nullptr;
}

// Implementations:
static inline void* play_alarm_sound_thread(void* argument) {
    // Compiler:
    (void) argument;

    // Command:
    if (system("aplay ./assets/alarm.wav") != 0) {
        std::cout << "[!] Error playing alarm sound!" << std::endl;
    }

    // Variables (Assignment):
    // Thread:
    pthread_t reset_thread_id;

    if (pthread_create(&reset_thread_id, nullptr, reset_alarm_flag_thread, nullptr) == 0) {
        pthread_detach(reset_thread_id);
    }

    // Logic:
    return nullptr;
}

static inline void* play_gain_sound_thread(void* argument) {
    // Compiler:
    (void) argument;

    // Command:
    if (system("aplay ./assets/gain.wav") != 0) {
        std::cout << "[!] Error playing gain sound!" << std::endl;
    }

    // Variables (Assignment):
    // Thread:
    pthread_t reset_thread_id;

    if (pthread_create(&reset_thread_id, nullptr, reset_gain_flag_thread, nullptr) == 0) {
        pthread_detach(reset_thread_id);
    }

    // Logic:
    return nullptr;
}

static inline void play_alarm_sound(void) {
    // Variables (Assignment):
    // Expected:
    bool expected = false;

    // Validation:
    if (!playing_alarm.compare_exchange_strong(expected, true)) {
        return;
    }

    // Variables (Assignment):
    // Thread:
    pthread_t alarm_thread_id;

    if (pthread_create(&alarm_thread_id, nullptr, play_alarm_sound_thread, nullptr) != 0) {
        // Logging:
        std::cout << "[!] Error creating thread for alarm sound!" << std::endl;

        // State:
        playing_alarm = false;

        // Logic:
        return;
    }

    if (pthread_detach(alarm_thread_id) != 0) {
        // Logging:
        std::cout << "[!] Error detaching thread!" << std::endl;

        // State:
        playing_alarm = false;

        // Logic:
        return;
    }
}

static inline void play_gain_sound(void) {
    // Variables (Assignment):
    // Expected:
    bool expected = false;

    // Validation:
    if (!playing_gain.compare_exchange_strong(expected, true)) {
        return;
    }

    // Variables (Assignment):
    // Thread:
    pthread_t gain_sound_thread_id;

    if (pthread_create(&gain_sound_thread_id, nullptr, play_gain_sound_thread, nullptr) != 0) {
        // Logging:
        std::cout << "[!] Error creating thread for gain sound!" << std::endl;

        // State:
        playing_gain = false;

        // Logic:
        return;
    }

    if (pthread_detach(gain_sound_thread_id) != 0) {
        // Logging:
        std::cout << "[!] Error detaching thread!" << std::endl;

        // State:
        playing_gain = false;

        // Logic:
        return;
    }
}

// Guard:
#endif /** __SOUND_UTILITIES_HPP__ */