
// TODO: GET RID OF MAGIC NUMBERS

#include "voicing.h"

Voicing::Voicing(Note _soprano, Note _alto, Note _tenor, Note _bass) : 
    voices{ {_soprano, _alto, _tenor, _bass} } {}

Note &Voicing::getSoprano() {
    return voices[0];
}

Note &Voicing::getAlto() {
    return voices[1];
}

Note &Voicing::getTenor() {
    return voices[2];
}

Note &Voicing::getBass() {
    return voices[3];
}

Note &Voicing::at(int idx) {
    return voices[idx];
}


bool Voicing::isValidVoicing() {
    bool voicesClose = getAlto().distanceTo(getSoprano()) <= 7 &&
                       getTenor().distanceTo(getAlto()) <= 7;
    bool voicesInOrder = getSoprano() > getAlto() &&
                         getAlto() > getTenor() &&
                         getTenor() > getBass();
    return voicesClose && voicesInOrder;
}

bool Voicing::isInRange(int key) {
    // soprano
    int sopranoMidiNum = getSoprano().toMidiNumber(key);
    int altoMidiNum = getAlto().toMidiNumber(key);
    int tenorMidiNum = getTenor().toMidiNumber(key);
    int bassMidiNum = getBass().toMidiNumber(key);
    if (sopranoMidiNum < SOPRANO_MIN || SOPRANO_MAX < sopranoMidiNum) {
        return false;
    }
    if (altoMidiNum < ALTO_MIN || ALTO_MAX < altoMidiNum) {
        return false;
    }
    if (tenorMidiNum < TENOR_MIN || TENOR_MAX < tenorMidiNum) {
        return false;
    }
    if (bassMidiNum < BASS_MIN || BASS_MAX < bassMidiNum) {
        return false;
    }
    return true;
}

/******************************* InterVoicing Constraints ******************************/

bool isParallelFifth(Voicing v1, Voicing v2) {
    for (int i = 0; i < NOTES_PER_VOICING; ++i) {
        for (int j = i + 1; j < NOTES_PER_VOICING; ++j) {
            int v1Interval = (v1.at(j)).distanceTo(v1.at(i));
            int v2Interval = (v2.at(j)).distanceTo(v2.at(i));
            if (v1Interval == 4 && v2Interval == 4) {
                return true;
            }
        }
    }
    return false;
}

bool isVoiceCrossing(Voicing v1, Voicing v2) {
    for (int i = 0; i < NOTES_PER_VOICING - 1; ++i) {
        if (v2.at(i + 1) > v1.at(i)) {
            return true;
        }
    }
    return false;
}

bool isVoiceSkipLarge(Voicing v1, Voicing v2) {
    for (int i = 0; i < NOTES_PER_VOICING; ++i) {
        Note prevNote = v1.at(i);
        Note nextNote = v2.at(i);
        // No more than P4
        if (abs(prevNote.distanceTo(nextNote)) > 4) {
            return true;
        }
    }
    return false;
}

std::vector<VoicingPredicate> interVoicingConstraints = {
    isParallelFifth,
    isVoiceCrossing,
    isVoiceSkipLarge
};

bool satisfiesAll(Voicing v1, Voicing v2) {
    for (VoicingPredicate pred : interVoicingConstraints) {
        if (pred(v1, v2)) {
            return false;
        }
    }
    return true;
}
