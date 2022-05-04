
#include "chord.h"

Chord::Chord(uint8_t _bassScaleDegree, uint8_t _degrees[3], std::string _chordName) 
: chordName(_chordName) {
    for (int i = 0; i < NOTES_PER_CHORD; ++i) {
        degrees[i] = _degrees[i];
    }
    bassScaleDegree = _bassScaleDegree;
}

bool Chord::operator==(const Chord &chord) {
    for (int i = 0; i < NOTES_PER_CHORD; ++i) {
        if (degrees[i] != chord.degrees[i]) {
            return false;
        }
    }
    return true;
}

bool Chord::isPartOfChord(Note note) {
    for (int i = 0; i < NOTES_PER_CHORD; ++i) {
        if (note.scaleDegree == degrees[i]) {
            return true;
        }
    }
    return false;
}

std::string Chord::toString() {
    return chordName;
}

bool Chord::isVoicingPartOfChord(Voicing voicing) {
    if (!isPartOfChord(voicing.getTenor())) {
        return false;
    }
    if (!isPartOfChord(voicing.getAlto())) {
        return false;
    }
    if (!isPartOfChord(voicing.getSoprano())) {
        return false;
    }

    return true;
}

bool Chord::isBassDegreeValid(Note bass) {
    if (bass.scaleDegree != degrees[0]) {
        return false;
    }
    return true;
}

bool Chord::isThirdCountValid(Voicing voicing) {
    int thirdOfChordCount = 0;
    for (int i = 0; i < NOTES_PER_VOICING; ++i) {
        Note currNote = voicing.voices[i];
        if (currNote.scaleDegree == degrees[1]) {
            ++thirdOfChordCount;
        }
    }
    return thirdOfChordCount == 1;
}

bool Chord::isValidChord(Voicing voicing) {
    // Bass must be the first scale degree of a chord.
    // All notes in the voicings must be part of the chord.
    // Make sure third of the chord appears once.

    return isBassDegreeValid(voicing.getBass()) && isVoicingPartOfChord(voicing) 
        && isThirdCountValid(voicing);
}

/* Global variables */

uint8_t degreesI[] = {1, 3, 5};
Chord I = Chord(0, degreesI, "I");

uint8_t degreesIV[] = {4, 6, 1};
Chord IV = Chord(0, degreesIV, "IV");

uint8_t degreesV[] = {5, 7, 2};
Chord V = Chord(0, degreesV, "V");

std::vector<Chord> chordLst = {I, IV, V};

/****************************** Chord Constraints **************************************/

bool isRetrogression(Chord ch1, Chord ch2) {
    return ch1 == V && ch2 == IV;
}

/* Not an actual rule, enforced just to ensure variety. */
bool isRepeatedChord(Chord ch1, Chord ch2) {
    return ch1 == ch2;
}

std::vector<ChordPredicate> chordConstraints = {
    isRetrogression,
    isRepeatedChord
};
