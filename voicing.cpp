
#include "voicing.h"

Voicing::Voicing(Note _soprano, Note _alto, Note _tenor, Note _bass) : soprano(_soprano.scaleDegree, _soprano.relativeOctave),
    alto(_alto.scaleDegree, _alto.relativeOctave),
    tenor(_tenor.scaleDegree, _tenor.relativeOctave),
    bass(_bass.scaleDegree, _bass.relativeOctave) {}

bool Voicing::isValidVoicing() {
    // TODO: Check for ranges.
    return soprano.isHigherThan(alto) &&
           alto.isHigherThan(tenor) &&
           tenor.isHigherThan(bass);
}
