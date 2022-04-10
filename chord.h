
#pragma once

#include <cstdint>
#include <stdbool.h>
#include <voicing.h>

// TODO: For chords outside this template, we can inherit from this class.
class Chord {
public:
    Chord(uint8_t _bassScaleDegree, uint8_t _degrees[3]);

    bool isValidChord(Voicing voicing);

private:
    uint8_t bassScaleDegree;
    uint8_t degrees[3];
};

uint8_t degreesI[] = {1, 3, 5};
Chord I = Chord(0, degreesI);

uint8_t degreesIV[] = {4, 6, 1};
Chord IV = Chord(0, degreesIV);

uint8_t degreesV[] = {5, 7, 2};
Chord V = Chord(0, degreesV);
