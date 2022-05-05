
#pragma once

#include <vector>
#include "note.h"
#include "voicing.h"

void readKeyAndSopranoLine(const char *filepath, char &key, std::vector<Note> &sopranoLine);
void writeVoicings(char *filepath, std::vector<Voicing> voicings);
