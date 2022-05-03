// Currently not used. Will be added to in the future.

#include "voicing.h"

virtual class InterVoicingConstraint {
public:
    bool isValid(Voicing v1, voicing v2);
}

// inherit to make each constraint
// - parallel fifths
// - voice crossings
// - no large skips

// list of all constrants


