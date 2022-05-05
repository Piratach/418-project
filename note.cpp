
#include "note.h"
#include <unordered_map>

std::unordered_map<int, int> halfStepsToScaleDegree = {
    {0, 1}, {2, 2}, {4, 3}, {5, 4}, {7, 5}, {9, 6}, {11, 7}
};

std::unordered_map<int, int> scaleDegreeToHalfSteps = {
    {1, 0}, {2, 2}, {3, 4}, {4, 5}, {5, 7}, {6, 9}, {7, 11}
};

Note::Note(uint8_t _scaleDegree, uint8_t _relativeOctave) {
    scaleDegree = _scaleDegree;
    relativeOctave = _relativeOctave;
}

int keyOffset(int key) {
    return ((key * 7) % 12 + 12) % 12; // additional modulo accounts for negative keys
}

Note Note::fromMidiNumber(int key, int midiNumber) {
   int offset = keyOffset(key);
   int normalizedMidiNumber = midiNumber - offset;
   int halfSteps = normalizedMidiNumber % 12;
   int scaleDegree = halfStepsToScaleDegree[halfSteps];
   int octave = normalizedMidiNumber / 12 - 1;
   // printf("%d, %d, %d, %d, %d, %d, %d", midiNumber, key, offset, normalizedMidiNumber, halfSteps, scaleDegree, octave);
   return Note(scaleDegree, octave);
}

static Note highestNoteNotAbove(int key, int midiNumber) {
    Note note = Note::fromMidiNumber(key, midiNumber);
    while (note.scaleDegree == 0) {
        note = Note::fromMidiNumber(key, --midiNumber);
    }
    return note;
}

static Note lowestNoteNotBelow(int key, int midiNumber) {
    Note note = Note::fromMidiNumber(key, midiNumber);
    while (note.scaleDegree == 0) {
        note = Note::fromMidiNumber(key, ++midiNumber);
    }
    return note;
}

std::string Note::toString() {
    std::string s1 = "(scaleDegree: " + std::to_string(scaleDegree);
    std::string s2 = ", relativeOctave: " + std::to_string(relativeOctave) + ")";
    return s1 + s2;
}

int Note::distanceTo(Note &note) {
    int degreeDiff = note.scaleDegree - scaleDegree;
    int octaveDiff = note.relativeOctave - relativeOctave;
    return degreeDiff + (octaveDiff * 7);
}

bool Note::operator>(const Note &note) {
    return relativeOctave > note.relativeOctave || 
        (relativeOctave == note.relativeOctave && 
         scaleDegree > note.scaleDegree);
}

bool Note::operator<(const Note &note) {
    return relativeOctave < note.relativeOctave || 
        (relativeOctave == note.relativeOctave && 
         scaleDegree < note.scaleDegree);
}

Note Note::operator++() {
    if (scaleDegree == 7) {
        scaleDegree = 0;
        relativeOctave = relativeOctave + 1;
    } else {
        scaleDegree = scaleDegree + 1;
    }
    return Note{scaleDegree, relativeOctave};
}


int Note::toMidiNumber(int key) {
    int halfSteps = scaleDegreeToHalfSteps[scaleDegree];
    int normalizedMidiNumber = 12 * (relativeOctave + 1) + halfSteps;
    return normalizedMidiNumber + keyOffset(key);
}
