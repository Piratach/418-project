
#include "chord.h"

Chord::Chord(uint8_t _bassScaleDegree, uint8_t _degrees[3]) {
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
