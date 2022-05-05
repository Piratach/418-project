#include "solver.h"

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

std::vector<std::vector<Chord>> getChordProgressions(std::vector<Note> melodyLine) {
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

std::vector<Voicing> getPossibleVoicings(Chord chord, Note soprano, int key) {
    std::vector<Voicing> possibleVoicings;

    Note bassMin = Note::highestNoteNotAbove(key, BASS_MIN);
    Note tenorMin = Note::highestNoteNotAbove(key, TENOR_MIN);
    Note altoMin = Note::highestNoteNotAbove(key, ALTO_MIN);

    Note bassMax = Note::lowestNoteNotBelow(key, BASS_MAX);
    Note tenorMax = Note::lowestNoteNotBelow(key, TENOR_MAX);
    Note altoMax = Note::lowestNoteNotBelow(key, ALTO_MAX);

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

    return possibleVoicings;
}

std::vector<std::vector<Voicing>> solver(std::vector<Note> melodyLine,
        std::vector<Chord> chordProg, int key) {
    std::vector<std::vector<Voicing>> solution{std::vector<Voicing>()}; 

    // For each note in the soprano
    for (int i = 0; i < melodyLine.size(); ++i) {
        Chord currChord = chordProg[i];
        Note sopranoNote = melodyLine[i];

        std::vector<std::vector<Voicing>> newVoicings;
        std::vector<Voicing> possibleVoicings = getPossibleVoicings(currChord,
                sopranoNote, key);

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

