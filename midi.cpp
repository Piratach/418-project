#include <stdio.h>
#include "midifile/MidiFile.h"
#include "midi.h"

void readKeyAndSopranoLine(const char *filepath, char &key, std::vector<Note> &sopranoLine) {
    // printf("Reading file: %s...\n", filepath);
    smf::MidiFile midifile(filepath);
    smf::MidiEventList track = midifile[0];
    int count = 0;
    for (int i = 0; i < track.getSize(); i++) {
        smf::MidiEvent event = track[i];
        if (event.isNoteOn()) {
            int noteNumber = event.getKeyNumber();
            Note note = Note::fromMidiNumber(key, noteNumber); 
            sopranoLine.push_back(note); 
        }
        else if (event.isMeta()) {
            if (event[1] == 0x59) { // is key signature message
                key = event[3]; // number of flats/sharps from -7 through 7 (flats negative, sharps positive)
            }
        }
    }
}
    
void writeVoicings(char *filepath, int key, std::vector<Voicing> voicings) {
    smf::MidiFile midifile;
    int ioi = 250;
    int channel = 0;
    int velocity = 100;
    const int numTracks = 4;

    for (int track = 0; track < 4; ++track) {
        midifile.addTrack();
    }

    int startTick = 0;
    for (Voicing voicing : voicings) {
        std::array<Note, numTracks> voices = voicing.voices;
        for (int track = 0; track < 4; ++track) {
            Note note = voices[track];
            midifile.addNoteOn(track, startTick, channel, note.toMidiNumber(key), velocity);
            midifile.addNoteOff(track, startTick + ioi, channel, note.toMidiNumber(key));
        }
    }
    midifile.sortTracks();

    if (filepath == "") {
        // TODO: Send results to standard out
    }
    else {
        midifile.write(filepath);
    }
}
