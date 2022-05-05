
#pragma once

#define NOTES_PER_VOICING 4
#define BASS_MIN 40
#define BASS_MAX 62
#define TENOR_MIN 48
#define TENOR_MAX 67
#define ALTO_MIN 55
#define ALTO_MAX 74
#define SOPRANO_MIN 60
#define SOPRANO_MAX 79

#include "note.h"
#include <array>
#include <cassert>
#include <stdbool.h>
#include <vector>

// Vertical slice
struct Voicing {
public:
    Voicing(Note _soprano, Note _alto, Note _tenor, Note _bass);

    // 1. Voices are in correct order.
    // 2. Each voice is in its correct range.
    bool isValidVoicing();

    bool isInRange(int key);

    // Get methods
    Note &getSoprano();
    Note &getAlto();
    Note &getTenor();
    Note &getBass();

    Note &at(int idx);


    std::array<Note, NOTES_PER_VOICING> voices;

};

bool isParallelFifth(Voicing v1, Voicing v2);
bool isParallelOctave(Voicing v1, Voicing v2);
bool isVoiceCrossing(Voicing v1, Voicing v2);
bool isVoiceSkipLarge(Voicing v1, Voicing v2);

typedef bool (*VoicingPredicate)(Voicing, Voicing);
extern std::vector<VoicingPredicate> interVoicingConstraints;

bool satisfiesAll(Voicing v1, Voicing v2);
