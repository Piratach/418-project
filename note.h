
#pragma once

#include <cstdint>
#include <stdbool.h>
// scale degree (relative to key, between 1-7 inclusive)
// octave (relative to key/vocal ranges, between 0-6)

struct Note {

    Note(uint8_t _scaleDegree, uint8_t _relativeOctave);

    Note fromMidiNumber(int key, int midiNumber);

    bool operator>(const Note &note);

    int toMidiNumber(int key);

    bool isHigherThan(Note &note);

    // TODO: Maybe combine into one variable and use a mask.
    uint8_t scaleDegree;
    uint8_t relativeOctave;
};
