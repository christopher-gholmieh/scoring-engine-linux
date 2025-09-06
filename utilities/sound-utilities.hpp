// Written by: Chrisotpher Gholmieh
// Guards:
#ifndef __SOUND_UTILITIES_HPP__
#define __SOUND_UTILITIES_HPP__

// Headers:
// Core:
#include <iostream>


// Prototypes:
static inline void play_alarm_sound(void);
static inline void play_gain_sound(void);


// Implementations:
static inline void play_alarm_sound(void) {
    if (system("aplay ./assets/alarm.wav") != 0) {
        std::cout << "[!] Error playing alarm sound!" << std::endl;
    }
}

static inline void play_gain_sound(void) {
    if (system("aplay ./assets/gain.wav") != 0) {
        std::cout << "[!] Error playing gain sound!" << std::endl;
    }
}

// Guard:
#endif /** __SOUND_UTILITIES_HPP__ */