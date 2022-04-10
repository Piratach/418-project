
#pragma once

#include <note.h>
#include <stdbool.h>

// Vertical slice
struct Voicing {

    Voicing(Note _soprano, Note _alto, Note _tenor, Note _bass);

    // 1. Voices are in correct order.
    // 2. Each voice is in its correct range.
    bool isValidVoicing();

    Note soprano;
    Note alto;
    Note tenor;
    Note bass;
};
