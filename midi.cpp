#include <stdio.h>
#include "midifile/MidiFile.h"
#include "midi.h"

std::vector<int> getSopranoNoteNumbers(const char *filepath, char &keySignature) {
    printf("Reading file: %s...\n", filepath);
    smf::MidiFile midifile(filepath);
    smf::MidiEventList track = midifile[0];
    std::vector<int> noteNumbers;
    int count = 0;
    for (int i = 0; i < track.getSize(); i++) {
        smf::MidiEvent event = track[i];
        if (event.isNoteOn()) {
           int pitch = event.getKeyNumber();
           noteNumbers.push_back(pitch);
        }
        else if (event.isMeta()) {
            if (event[1] == 0x59) { // is key signature message
                keySignature = event[3]; // number of flats/sharps from -7 through 7 (flats negative, sharps positive)
            }
        }
    }
    return noteNumbers;
}

void readKeyAndSopranoLine(const char *filepath, int &key, std::vector<Note> sopranoLine) {
    printf("Reading file: %s...\n", filepath);
    smf::MidiFile midifile(filepath);
    smf::MidiEventList track = midifile[0];
    std::vector<Note> sopranoLine;
    int count = 0;
    for (int i = 0; i < track.getSize(); i++) {
        smf::MidiEvent event = track[i];
        if (event.isNoteOn()) {
           int pitch = event.getKeyNumber();
           noteNumbers.push_back(pitch);
        }
        else if (event.isMeta()) {
            if (event[1] == 0x59) { // is key signature message
                keySignature = event[3]; // number of flats/sharps from -7 through 7 (flats negative, sharps positive)
            }
        }
    }
    return noteNumbers;
}
    
void writeVoicings(char *filepath, std::vector<Voicing> voicings);
