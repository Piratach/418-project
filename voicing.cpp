
#include "voicing.h"

Voicing::Voicing(Note _soprano, Note _alto, Note _tenor, Note _bass) : 
    voices{ {_soprano, _alto, _tenor, _bass} } {}

Note &Voicing::getSoprano() {
    return voices[0];
}
Note &Voicing::getAlto() {
    return voices[1];
}
Note &Voicing::getTenor() {
    return voices[2];
}
Note &Voicing::getBass() {
    return voices[3];
}

bool Voicing::isValidVoicing() {
    // TODO: Check for ranges.
    return getSoprano().isHigherThan(getAlto()) &&
           getAlto().isHigherThan(getTenor()) &&
           getTenor().isHigherThan(getBass());
}
