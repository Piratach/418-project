
#pragma once

// TODO: Will have to change for V7.
#define NOTES_PER_CHORD 3

#include <cstdint>
#include <stdbool.h>
#include <string>
#include <vector>
#include "voicing.h"

// TODO: For chords outside this template, we can inherit from this class.
class Chord {
public:
    Chord(uint8_t _bassScaleDegree, uint8_t _degrees[NOTES_PER_CHORD],
            std::string _chordName);

    bool operator==(const Chord &chord);

    // TODO: change name
    bool isValidChord(Voicing voicing);
    bool isPartOfChord(Note note);

    std::string toString();

private:
    bool isVoicingPartOfChord(Voicing voicing);
    bool isBassDegreeValid(Note note);
    bool isThirdCountValid(Voicing voicing);

    std::string chordName;
    uint8_t bassScaleDegree;
    uint8_t degrees[NOTES_PER_CHORD];
};

/******************************* List of Chords ****************************************/
extern Chord I;
extern Chord IV;
extern Chord V;
extern std::vector<Chord> chordLst;

/****************************** Chord Constraints **************************************/

bool isRetrogression(Chord ch1, Chord ch2);

typedef bool (*ChordPredicate)(Chord, Chord);
extern std::vector<ChordPredicate> chordConstraints;
