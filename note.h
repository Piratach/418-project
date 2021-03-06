
#pragma once

#include <cmath>
#include <cstdint>
#include <stdbool.h>
#include <string>

// scale degree (relative to key, between 1-7 inclusive)
// octave (relative to key/vocal ranges, between 0-6)

struct Note {

    Note(uint8_t _scaleDegree, uint8_t _relativeOctave);

    static Note fromMidiNumber(int key, int midiNumber);
    static Note highestNoteNotAbove(int key, int midiNumber);
    static Note lowestNoteNotBelow(int key, int midiNumber);

    bool operator>(const Note &note);
    bool operator<(const Note &note);

    Note operator++();

    std::string toString();

    int toMidiNumber(int key);

    int distanceTo(Note &note);

    // TODO: Maybe combine into one variable and use a mask.
    uint8_t scaleDegree;
    uint8_t relativeOctave;
};
