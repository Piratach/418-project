
#include "note.h"

Note::Note(uint8_t _scaleDegree, uint8_t _relativeOctave) {
    scaleDegree = _scaleDegree;
    relativeOctave = _relativeOctave;
}

Note::Note(int key, int midiNumber) {
   // TODO: everything 
}

// TODO: operator overload >, (and >=, <, <= if it's easy)
bool Note::operator>(const Note &note) {
    return relativeOctave > note.relativeOctave || 
        (relativeOctave == note.relativeOctave && 
         scaleDegree > note.scaleDegree);
}

int Note::toMidiNumber(int key) {
    return -1;
}
