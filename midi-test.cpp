#include "midifile/MidiFile.h"
#include <stdio.h>
#include "midi.h"

// #include <iostream>
// #include <iomanip>

// using namespace std;
// using namespace smf;

std::vector<int> getPitches(const char *filepath, double &tempoBPM, char &keySignature) {
    printf("Reading file: %s...\n", filepath);
    smf::MidiFile midifile("test.mid");
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
            if (event.isTempo()) {
                tempoBPM = event.getTempoBPM();
            }
            else if (event[1] == 0x59) { // is key signature message
                keySignature = event[3]; // number of flats/sharps from -7 through 7 (flats negative, sharps positive)
            }
        }
    }
    return noteNumbers;
}

void getNoteName(int noteNumber, char *result) {
    int octave = noteNumber / 12 - 1; // 21...23 => 0, 24...35 => 1, 36...47 => 2
    char const *noteNames[12] = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    char const *noteName = noteNames[noteNumber % 12];
    sprintf(result, "%s%d", noteName, octave);
}

void outputMidiFileInfo(const char *filepath) {
    double tempoBPM;
    char keySignature;
    std::vector<int> noteNumbers = getPitches(filepath, tempoBPM, keySignature);
    printf("Tempo (BPM): %f\n", tempoBPM);
    printf("Key Signature: %d\n", keySignature);
    for (int i = 0; i < noteNumbers.size(); i++) {
        char *noteName = (char *) malloc(4);
        getNoteName(noteNumbers[i], noteName);
        printf("Pitch %d: %s\n", i, noteName);
    }
}

int main() { 
    const char *filepath = "test.mid";
    outputMidiFileInfo(filepath);
    int key;
    std::vector<Note> sopranoLine;
    readKeyAndSopranoLine(filepath, key, sopranoLine);

    return 0;
}
