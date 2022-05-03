
#pragma once

#define NOTES_PER_VOICING 4

#include "note.h"
#include <array>
#include <vector>
#include <stdbool.h>

// Vertical slice
struct Voicing {
public:
    Voicing(Note _soprano, Note _alto, Note _tenor, Note _bass);

    // 1. Voices are in correct order.
    // 2. Each voice is in its correct range.
    bool isValidVoicing();

    // Get methods
    Note &getSoprano();
    Note &getAlto();
    Note &getTenor();
    Note &getBass();

    Note &at(int idx);

    int getVoicingInterval(int idx1, int idx2);

    std::array<Note, NOTES_PER_VOICING> voices;
};

bool isParallelFifth(Voicing v1, Voicing v2);
bool isVoiceCrossing(Voicing v1, Voicing v2);
bool isVoiceSkipLarge(Voicing v1, Voicing v2);

typedef bool (*VoicingPredicate)(Voicing, Voicing);
extern std::vector<VoicingPredicate> interVoicingConstraints;
