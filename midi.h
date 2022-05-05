
#pragma once

#include <vector>
#include "note.h"
#include "voicing.h"

void readKeyAndSopranoLine(const char *filepath, int &key, std::vector<Note> sopranoLine);
void writeVoicings(char *filepath, std::vector<Voicing> voicings);
