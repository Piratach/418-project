#include "solver.h"
#include <omp.h>
#define GRANULARITY_LIMIT 3

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

std::vector<std::vector<Chord>> getChordProgressions(std::vector<Note> melodyLine, int numThreads) {
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


std::vector<std::vector<Voicing>> solverSequential(std::vector<Note> melodyLine,
        std::vector<Chord> chordProg, int key, int numThreads, int startIndex, int endIndex) {
    std::vector<std::vector<Voicing>> solution{std::vector<Voicing>()}; 

    // For each note in the soprano
    for (int i = startIndex; i < endIndex; ++i) {
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


std::vector<std::vector<Voicing>> merge(std::vector<std::vector<Voicing>> fst, std::vector<std::vector<Voicing>> snd) {
    std::vector<std::vector<Voicing>> mergedSol;
    for (std::vector<Voicing> v1 : fst) {
        for (std::vector<Voicing> v2 : snd) {
            if (v1.empty() || v2.empty() || satisfiesAll(v1.back(), v2.front())) {
                std::vector<Voicing> v1v2Merged;
                v1v2Merged.reserve(v1.size() + v2.size());
                v1v2Merged.insert(v1v2Merged.end(), v1.begin(), v1.end());
                v1v2Merged.insert(v1v2Merged.end(), v2.begin(), v2.end());
                mergedSol.push_back(v1v2Merged);
            }
        }
    }
    return mergedSol;
}

std::vector<std::vector<Voicing>> solverRecursive(std::vector<Note> melodyLine,
        std::vector<Chord> chordProg, int key, int numThreads, int startIndex, int endIndex) {
#pragma omp parallel
    std::vector<std::vector<Voicing>> solution{std::vector<Voicing>()}; 

    int length = endIndex - startIndex;

    if (length < GRANULARITY_LIMIT) {
        printf("gran limit: %d\n", omp_get_thread_num());
        return solverSequential(melodyLine, chordProg, key, numThreads, startIndex, endIndex);
    }
    printf("not gran limit\n");

    std::vector<std::vector<Voicing>> fstHalfSol;
    std::vector<std::vector<Voicing>> sndHalfSol;

#pragma omp task shared(melodyLine, chordProg)
    fstHalfSol = solverRecursive(melodyLine, chordProg, key, numThreads, startIndex, startIndex + length / 2);

    sndHalfSol = solverRecursive(melodyLine, chordProg, key, numThreads, startIndex + length / 2, endIndex);

#pragma omp taskwait
    return merge(fstHalfSol, sndHalfSol);
}


std::vector<std::vector<Voicing>> solver(std::vector<Note> melodyLine,
        std::vector<Chord> chordProg, int key, int numThreads) {
    // return solverSequential(melodyLine, chordProg, key, numThreads, 0, melodyLine.size());
    return solverRecursive(melodyLine, chordProg, key, numThreads, 0, melodyLine.size());
}
