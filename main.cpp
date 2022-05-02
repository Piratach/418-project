#include <vector>
#include <stdio.h>

#include "chord.h"
#include "constraint.h"
#include "note.h"
#include "voicing.h"

uint8_t degreesI[] = {1, 3, 5};
Chord I = Chord(0, degreesI);

uint8_t degreesIV[] = {4, 6, 1};
Chord IV = Chord(0, degreesIV);

uint8_t degreesV[] = {5, 7, 2};
Chord V = Chord(0, degreesV);

std::vector<Chord> chordLst = {I, IV, V};

/** Pseudocode
 * 1. initialize chordProgs as [[]]
 * 2. for sopranoNote in melodyLine
 *    a. initialize newChordProgs as []
 *    b. get possibleChords from sopranoNote
 *    c. for each possibleChord in possibleChords
 *       i. for each prog in chordProgs
 *          1). if (isStillValidProgression(prog + possibleChord))
 *              > newChordProgs.append(prog + possibleChord)
 *    d. chordProgs = newChordProgs
 * 3. return chordProgs
 */

// TODO: move somewhere else
std::vector<Chord> getPossibleChords(Note note) {
    std::vector<Chord> possibleChords;
    for (Chord chord : chordLst) {
        if (chord.isPartOfChord(note)) {
            possibleChords.push_back(chord);
        }
    }
    return possibleChords;
}

bool isStillValidProgression(std::vector<Chord> prog, Chord possibleChord) {
    // V > IV constraint check
    if (prog.back() == V && possibleChord == IV) {
        return false;
    }

    // Voicing constraints check (no overlapping, small steps etc)
    return true;
}

std::vector<std::vector<Chord>> solverHelper(std::vector<Note> melodyLine) {
    // [[]]
    std::vector<std::vector<Chord>> chordProgs{std::vector<Chord>()}; 

    // For each note in the soprano
    for (Note sopranoNote : melodyLine) {
        std::vector<std::vector<Chord>> newChordProgs;
        std::vector<Chord> possibleChords = getPossibleChords(sopranoNote);
        // For each possible chord
        for (Chord possibleChord : possibleChords) {
            // For each prog in our current chord progression
            for (std::vector<Chord> prog : chordProgs) {
                if (isStillValidProgression(prog, possibleChord)) {
                    prog.push_back(possibleChord);
                    newChordProgs.push_back(prog);
                }
            }
        }
        chordProgs = newChordProgs;
    }

    return chordProgs;
}

/** Pseudocode (for one chord progression)
 * 1. initialize voicings as [[]]
 * 2. for (sopranoNote, chord) in (melodyLine, chordProgs)
 *    a. initialize newVoicings as []
 *    b. for auto voicing in voicings
 *       i). get possibleVoicings from helper function(previousVoicing, chord, sopranoNote) -- look at slide 18
 *       ii). 
 */
std::vector<std::vector<Voicing>> solver(std::vector<Note> melodyLine,
        std::vector<std::vector<Chord>> chordProgs) {
    std::vector<std::vector<Voicing>> solution;
    return solution;
}

int main() {
    /* Takes in MIDI input
     *    - has number of vertical slices (voicings)
     *    - allocates that number
     *    - get key
     *    - convert MIDI input into an array of notes
     */

    // Here we have a container of notes (melody/soprano)

    /* Solver helper
     *    - takes in array of notes, output an array of arrays of chords
     *    - initial thought: use a vector because it is dynamically sized
     */

    // We have a valid array of array of chords that satisfy the hard constraints

    /* Solve
     *    - given an array of notes (soprano line) and array of chords, output an array of arrays of voicings
     */
    printf("Hello World!\n");
    return 0;
}
