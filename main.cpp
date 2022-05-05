
#include <chrono>
#include <stdio.h>
#include <vector>

#include "chord.h"
#include "note.h"
#include "voicing.h"
#include "midi.h"

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

    if (prog.empty()) {
        return true;
    }

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

std::vector<Voicing> getPossibleVoicings(Chord chord, Note soprano/*, int key*/) {
    std::vector<Voicing> possibleVoicings;

    int key = -2;
    Note bassMin = Note::fromMidiNumber(BASS_MIN, key);
    Note tenorMin = Note::fromMidiNumber(TENOR_MIN, key);
    Note altoMin = Note::fromMidiNumber(ALTO_MIN, key);

    Note bassMax = Note::fromMidiNumber(BASS_MAX, key);
    Note tenorMax = Note::fromMidiNumber(TENOR_MAX, key);
    Note altoMax = Note::fromMidiNumber(ALTO_MAX, key);

    // TODO: this doesn't work currently.
    for (Note bass = bassMin; bass < bassMax; ++bass) {
        for (Note tenor = tenorMin; tenor < tenorMax; ++tenor) {
            for (Note alto = altoMin; alto < altoMax; ++alto) {
                Voicing currVoicing{soprano, alto, tenor, bass};

                if (currVoicing.isInRange(key) && 
                    currVoicing.isValidVoicing() && 
                    chord.isValidChord(currVoicing)) {
                    possibleVoicings.push_back(currVoicing);
                }
            }
        }
    }

    // Note bassLimit{1, 4};  // ranges from (1, 0) to (1, 4)
    // Note tenorLimit{1, 5};
    // Note altoLimit{1, 6};
    // for (Note bass = Note::fromMidiNumber(BASS_MIN, key); bass < bassLimit; ++bass) {
        // for (Note tenor = Note{1, 1}; tenor < tenorLimit; ++tenor) {
            // for (Note alto = Note{1, 2}; alto < altoLimit; ++alto) {
                // Voicing currVoicing{soprano, alto, tenor, bass};

                // if (currVoicing.isInRange(key) && 
                    // currVoicing.isValidVoicing() && 
                    // chord.isValidChord(currVoicing)) {
                    // possibleVoicings.push_back(currVoicing);
                // }
            // }
        // }
    // }

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
                if (solutionVoicing.empty() || 
                        satisfiesAll(solutionVoicing.back(), possibleVoicing)) {
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

void printChordProg(std::vector<Chord> chordProg) {
    for (size_t i = 0; i < chordProg.size(); ++i) {
        Chord chord = chordProg[i];
        printf("%s", chord.toString().c_str());
        if (i < chordProg.size() - 1) {
            printf(" > ");
        } else {
            printf("\n");
        }
    }
}

int main() {
    using namespace std::chrono;
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::duration<double> dsec;

    auto initStart = Clock::now();
    double initTime = 0;

#if 0
    /* Make hardcoded melody line of Bb F Bb to use for testing */
    Note sop1{1, 4};
    Note sop2{5, 4};
    Note sop3{1, 4};
    std::vector<Note> melodyLine{sop1, sop2, sop3};


#endif
    char key;
    std::vector<Note> melodyLine;
    readKeyAndSopranoLine("test.mid", key, melodyLine);

    int count = 0;
    for (Note n : melodyLine) {
        printf("Note %d: %s\n", count, n.toString().c_str());
        count++;
    }

    initTime += duration_cast<dsec>(Clock::now() - initStart).count();
    printf("Initialization Time: %lf.\n", initTime);

    auto computeStart = Clock::now();
    double computeTime = 0;

    std::vector<std::vector<Chord>> possibleChordProgs = solverHelper(melodyLine);

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

    size_t totalSize = 0;
    for (std::vector<Chord> chordProg : possibleChordProgs) {
        printChordProg(chordProg);
        std::vector<std::vector<Voicing>> solution = solver(melodyLine, chordProg);
        totalSize += solution.size();
        for (std::vector<Voicing> validVoicings : solution) {
            // for each voicing
            for (Voicing voicing : validVoicings) {
                for (int i = 0; i < NOTES_PER_VOICING; ++i) {
                    Note currNote = voicing.at(i);
                    printf("%s ", currNote.toString().c_str());
                }
            }
            printf("\n");
        }
        printf("-----------------------------\n");
    }
    printf("totalSize: %lu\n", totalSize);

    computeTime += duration_cast<dsec>(Clock::now() - computeStart).count();
    printf("Computation Time: %lf.\n", computeTime);
    printf("Total Time: %lf.\n", computeTime + initTime);

    return 0;
}
