
#pragma once

#include <cstdint>
#include <stdbool.h>
// scale degree (relative to key, between 1-7 inclusive)
// octave (relative to key/vocal ranges, between 0-6)

struct AbsolutePitch {
    struct PitchClass {
        enum PitchLetter { C, D, E, F, G, A, B };
        enum AccidentalType {
            FLAT,
            NATURAL,
            SHARP
        };

        PitchClass(PitchLetter _letter, AccidentalType _accidental);

        PitchLetter letter;
        AccidentalType accidental;
    };

    AbsolutePitch(PitchClass _pitch, uint8_t _absoluteOctave);

    PitchClass pitchType;
    uint8_t absoluteOctave;
};

struct Note {
    Note(uint8_t _scaleDegree, uint8_t _relativeOctave);
    Note(AbsolutePitch::PitchClass key, AbsolutePitch pitch);

    AbsolutePitch toAbsolutePitch(AbsolutePitch::PitchClass key);

    bool isHigherThan(Note &note);

    // TODO: Maybe combine into one variable and use a mask.
    uint8_t scaleDegree;
    uint8_t relativeOctave;
};
