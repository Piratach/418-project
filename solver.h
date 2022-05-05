#include <vector>

#include "chord.h"
#include "note.h"
#include "voicing.h"

std::vector<std::vector<Chord>> getChordProgressions(std::vector<Note> melodyLine);
std::vector<std::vector<Voicing>> solver(std::vector<Note> melodyLine,
        std::vector<Chord> chordProg, int key);
