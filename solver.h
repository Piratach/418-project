
#pragma once

#include <vector>

#include "chord.h"
#include "note.h"
#include "voicing.h"

// TODO: make optional argument for numThreads
std::vector<std::vector<Chord>> getChordProgressions(std::vector<Note> melodyLine, int numThreads = 1);
std::vector<std::vector<Voicing>> solver(std::vector<Note> melodyLine,
        std::vector<Chord> chordProg, int key, int numThreads = 1);
