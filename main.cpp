#include <vector>
#include <stdio.h>

#include "chord.h"
#include "note.h"
#include "voicing.h"

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

    Chord lastChord = prog.back();
    for (ChordPredicate pred : chordConstraints) {
        if (pred(lastChord, possibleChord)) {
            return false;
        }
    }

    return true;
}

std::vector<std::vector<Chord>> solverHelper(std::vector<Note> melodyLine) {
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
                    std::vector<Chord> newProg(prog);
                    newProg.push_back(possibleChord);
                    newChordProgs.push_back(newProg);
                }
            }
        }
        chordProgs = newChordProgs;
    }

    return chordProgs;
}

std::vector<Voicing> getPossibleVoicings(Chord chord, Note sopranoNote) {
    std::vector<Voicing> possibleVoicings;
    int key = -2;
    // G5 is 79, E2 is 40
    for (int bass = 40; bass < 79; ++bass) {
        for (int tenor = 40; tenor < 79; ++tenor) {
            for (int alto = 40; alto < 79; ++alto) {
                Voicing currVoicing{
                    sopranoNote,
                    Note::fromMidiNumber(key, alto),
                    Note::fromMidiNumber(key, tenor),
                    Note::fromMidiNumber(key, bass)
                };

                if (currVoicing.isValidVoicing() && chord.isValidChord(currVoicing)) {
                    possibleVoicings.push_back(currVoicing);
                }
            }
        }
    }

    return possibleVoicings;
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
        std::vector<Chord> chordProg) {
    std::vector<std::vector<Voicing>> solution{std::vector<Voicing>()}; 

    // For each note in the soprano
    for (int i = 0; i < melodyLine.size(); ++i) {
        Chord currChord = chordProg[i];
        Note sopranoNote = melodyLine[i];

        std::vector<std::vector<Voicing>> newVoicings;
        std::vector<Voicing> possibleVoicings = getPossibleVoicings(currChord, sopranoNote);

        // For each possible chord
        for (Voicing possibleVoicing : possibleVoicings) {
            // For each prog in our current chord progression
            for (std::vector<Voicing> solutionVoicing : solution) {
                if (satisfiesAll(solutionVoicing.back(), possibleVoicing)) {
                    std::vector<Voicing> solutionVoicingCopy(solutionVoicing);
                    solutionVoicingCopy.push_back(possibleVoicing);
                    newVoicings.push_back(solutionVoicingCopy);
                }
            }
        }
        solution = newVoicings;
    }

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
