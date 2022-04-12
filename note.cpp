
#include "note.h"

AbsolutePitch::PitchClass::PitchClass(PitchLetter _letter, AccidentalType _accidental) {
    letter = _letter;
    accidental = _accidental;
}

AbsolutePitch::AbsolutePitch(PitchClass _pitch, uint8_t _absoluteOctave) : pitchType(_pitch.letter, _pitch.accidental) {
    absoluteOctave = _absoluteOctave;
}

Note::Note(uint8_t _scaleDegree, uint8_t _relativeOctave) {
    scaleDegree = _scaleDegree;
    relativeOctave = _relativeOctave;
}

Note::Note(AbsolutePitch::PitchClass key, AbsolutePitch pitch) {
    if (pitch.pitchType.letter > key.letter) {
        scaleDegree = pitch.pitchType.letter - key.letter;
    } else {
        scaleDegree = key.letter - pitch.pitchType.letter;
    }

    // TODO: Fix relativeOctave
    relativeOctave = pitch.absoluteOctave;
}

bool Note::isHigherThan(Note &note) {
    return relativeOctave > note.relativeOctave || 
        (relativeOctave == note.relativeOctave && 
         scaleDegree > note.scaleDegree);
}
