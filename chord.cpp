
#include "chord.h"

// TODO: Remove magic numbers
Chord::Chord(uint8_t _bassScaleDegree, uint8_t _degrees[3]) {
    for (int i = 0; i < 3; ++i) {
        degrees[i] = _degrees[i];
    }
    bassScaleDegree = _bassScaleDegree;
}

bool Chord::isPartOfChord(Note note) {
    for (int i = 0; i < 3; ++i) {
        if (note.scaleDegree == degrees[i]) {
            return true;
        }
    }
    return false;
}

bool Chord::isValidChord(Voicing voicing) {
    // TODO: Make each of these into a helper function.
    // Bass must be the first scale degree of a chord.
    if (voicing.bass.scaleDegree != degrees[0]) {
        return false;
    }

    // TODO: Change voicing into a class with an internal array.
    //       Use get method to get each voice.
 
    // Check if voicing are all part of the chord.
    if (!isPartOfChord(voicing.tenor)) {
        return false;
    }
    if (!isPartOfChord(voicing.alto)) {
        return false;
    }
    if (!isPartOfChord(voicing.soprano)) {
        return false;
    }

    // Make sure third of the chord appears once.
    int thirdOfChordCount = 0;
    if (voicing.tenor.scaleDegree == degrees[1]) {
        ++thirdOfChordCount;
    }
    if (voicing.alto.scaleDegree == degrees[1]) {
        ++thirdOfChordCount;
    }
    if (voicing.soprano.scaleDegree == degrees[1]) {
        ++thirdOfChordCount;
    }

    return thirdOfChordCount == 1;
}
