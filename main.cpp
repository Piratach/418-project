
#include <chrono>
#include <omp.h>
#include <stdio.h>
#include <vector>

#include "chord.h"
#include "note.h"
#include "solver.h"
#include "voicing.h"
#include "midi.h"

/******************************* Debugging Functions ***********************************/

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

void printSolutionsPerChordProg(std::vector<std::vector<Voicing>> solution) {
    for (std::vector<Voicing> validVoicings : solution) {
        // for each voicing
        for (int i = 0; i < NOTES_PER_VOICING; ++i) {
            for (Voicing voicing : validVoicings) {
                Note currNote = voicing.at(i);
                printf("%s ", currNote.toString().c_str());
            }
            printf("\n");
        }
        printf("==\n");
    }
    printf("-----------------------------\n");
}

/***************************************************************************************/

int main() {
    using namespace std::chrono;
    typedef std::chrono::high_resolution_clock Clock;
    typedef std::chrono::duration<double> dsec;


    omp_set_num_threads(128);

    auto initStart = Clock::now();
    double initTime = 0;

    int iterations = 4;

    /* Make hardcoded melody line of Bb F Bb to use for testing */
// #if 0
    int key = 3;
    Note sop1{3, 4};
    Note sop2{4, 4};
    Note sop3{5, 4};
    std::vector<Note> melodyLine;

    for (int i = 0; i < iterations; ++i) {
        melodyLine.push_back(sop1);
        melodyLine.push_back(sop2);
        melodyLine.push_back(sop3);
    }

// #endif

    // char key;
    // std::vector<Note> melodyLine;
    // readKeyAndSopranoLine("midi-inputs/assn1.mid", key, melodyLine);

    // int count = 0;
    // for (Note n : melodyLine) {
        // printf("Note %d: %s\n", count, n.toString().c_str());
        // count++;
    // }

    initTime += duration_cast<dsec>(Clock::now() - initStart).count();
    printf("Initialization Time: %lf.\n", initTime);

    auto computeStart = Clock::now();
    double computeTime = 0;

    std::vector<std::vector<Chord>> possibleChordProgs = getChordProgressions(melodyLine);

    size_t totalSize = 0;

    int i;
#pragma omp parallel for default(shared) private(i) schedule(dynamic)
    for (i = 0; i < possibleChordProgs.size(); ++i) {
        std::vector<Chord> chordProg = possibleChordProgs[i];
    // for (std::vector<Chord> chordProg : possibleChordProgs) {
        // printChordProg(chordProg);

        std::vector<std::vector<Voicing>> solution = solver(melodyLine, chordProg, key);
        // totalSize += solution.size();
        // printf("%lu\n", solution.size());
        //printSolutionsPerChordProg(solution);
    }

    printf("totalSize: %lu\n", totalSize);

    computeTime += duration_cast<dsec>(Clock::now() - computeStart).count();
    printf("Computation Time: %lf.\n", computeTime);
    printf("Total Time: %lf.\n", computeTime + initTime);

    return 0;
}
