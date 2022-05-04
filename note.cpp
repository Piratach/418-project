
#include "note.h"
#include <unordered_map>

std::unordered_map<int, int> intervalToDegree = {
    {0, 1}, {2, 2}, {4, 3}, {5, 4}, {7, 5}, {9, 6}, {11, 7}
};

std::unordered_map<int, int> degreeToInterval = {
    {1, 0}, {2, 2}, {3, 4}, {4, 5}, {5, 7}, {6, 9}, {7, 11}
};

Note::Note(uint8_t _scaleDegree, uint8_t _relativeOctave) {
    scaleDegree = _scaleDegree;
    relativeOctave = _relativeOctave;
}

Note Note::fromMidiNumber(int key, int midiNumber) {
   int offset = (key * 7) % 12;   
   int newScaleDegree = intervalToDegree[(midiNumber - offset) % 12];
   int newRelativeOctave = ((midiNumber - 24) + offset) / 12;
   return Note(newScaleDegree, newRelativeOctave);
}

std::string Note::toString() {
    std::string s1 = "(scaleDegree: " + std::to_string(scaleDegree);
    std::string s2 = ", relativeOctave: " + std::to_string(relativeOctave);
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

int Note::toMidiNumber(int key) {
    int offset = (key * 7) % 12;
    return relativeOctave * 12 + 24 + offset;

}
