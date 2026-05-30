/**
 * @file Random.cpp
 * @brief Gestion du générateur aléatoire.
 */

#include "Random.h"
extern byte currentVelocity;

// number of pitch lower than the one to compute with uniform distribution
byte Random::count[NUM_SCALE][12];
byte Random::scales[NUM_SCALE][12];
byte Random::pitchs[NUM_SCALE][12] = { // i-th pitch in C
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},  // chromatic
    {0, 2, 4, 5, 7, 9, 11},                  // major
    {0, 2, 4, 7, 9},                         // pentatonic
    {0, 2, 3, 5, 7, 8, 11},                  // harmonic
};

int Random::scale_size[NUM_SCALE] = {12, 7, 5, 7};

/**
 * @brief Initialisation des tableaux.
 *
 * À partir des hauteurs par gamme, initialise le tableau
 * des gammes et les compteurs de notes.
 */
void Random::init_arrays() {
    for(int scale = 0; scale < NUM_SCALE; scale++) {
        for(int pitch = 0; pitch < 12; pitch++) {
            scales[scale][pitch] = 0;
        }
        for(int i = 0; i < scale_size[scale]; i++) {
            scales[scale][pitchs[scale][i]] = 1;
        }
        count[scale][0] = 1;
        for(int i = 1; i < 12; i++) {
            count[scale][i] = count[scale][i - 1] + scales[scale][i];
        }
    }
}

/**
 * @brief Retourne un nombre de notes.
 *
 * Pour une gamme donnée et une hauteur donnée, retourne
 * le nombre de notes inférieure ou égale à cette hauteur
 * dans cette gamme.
 */

byte Random::get_count(byte scale, byte note) {
    // lower or equal pitch 
    return count[scale][11] * (note / 12) + 
        count[scale][note % 12] -
        scales[scale][note % 12];
}

/**
 * @brief Retourne une hauteur de note.
 *
 * Pour une gamme donnée et un rang donné, retourne
 * la hauteur correspondante.
 */
byte Random::get_pitch(byte scale, byte rank) {
    return 12 * (rank / scale_size[scale]) + 
        pitchs[scale][rank % scale_size[scale]];
}

/**
 * @brief Retourne une hauteur de note aléatoire.
 *
 * Pour une gamme donnée et une tonalité donnée, retourne
 * une hauteur aléatoire comprise entre min (inclus) 
 * et max (exclus) pour un tirage uniforme.
 */
byte Random::rand_note(byte min, byte max, byte tone, byte scale) {
    int a = get_count(scale, min - tone + 12);
    int b = get_count(scale, max - tone + 12);
    if(a == b) {
        return 0;
    }
    // rank in the C range with uniform distribution
    int r = random(a, b);
    // pitch in tone
    int note = tone + get_pitch(scale, r);
    if(max <= note) {
        note -= 12;
    }
    return note;
}

/**
 * @brief Rotation de l'encodeur VALUE.
 *
 * Désactivé pour le gèle de la séquence.
 * */
void Random::handleRotate1(int8_t rotation) {
    if (currentIndex == FREEZE) {
        return;
    }
    if (currentIndex == LENGTH && parameters[FREEZE].value) {
        return;
    }
    switch (currentIndex) {
    case MIN:
        if (rotation > 0 &&
            parameters[currentIndex].buffer < parameters[MAX].value) {
            parameters[currentIndex].buffer++;
        } else if (rotation < 0 && parameters[currentIndex].min <
                                       parameters[currentIndex].buffer) {
            parameters[currentIndex].buffer--;
        }
        break;
    case MAX:
        if (rotation > 0 &&
            parameters[currentIndex].buffer < 
                parameters[currentIndex].max) {
            parameters[currentIndex].buffer++;
        } else if (rotation < 0 &&
                   parameters[MIN].value < 
                    parameters[currentIndex].buffer) {
            parameters[currentIndex].buffer--;
        }
        break;
    default:
        Module::handleRotate1(rotation);
        break;
    }
}

/**
 * @brief Pression sur l'encodeur VALUE.
 *
 * Active/désactive le canal de sortie, le gèle de la séquence
 * et les limites de notes.
 */
void Random::handlePress1() {
    switch (currentIndex) {
    case OUT:
        if (parameters[currentIndex].value != 0) {
            parameters[currentIndex].buffer = 0;
            out = parameters[currentIndex].value;
        } else {
            parameters[currentIndex].buffer = out;
        }
        break;
    case FREEZE:
        parameters[currentIndex].buffer = 
            1 - parameters[currentIndex].value;
        break;
    case MIN:
        parameters[currentIndex].buffer = NOTE_MIN;
        break;
    case MAX:
        parameters[currentIndex].buffer = NOTE_MAX;
        break;
    default:
        break;
    }
}

/**
 * @brief Gestion de l'horloge.
 *
 * Tous les 6 ticks, on génère une note aléatoire qu'on stocke dans 
 * un tableau qui sera joué quand la séquence sera gélée.
 */

void Random::handleClock() {
    if (parameters[OUT].value == 0) { // output channel is nul
        return;
    }
    if (parameters[LENGTH].value == 0) { // sequence length channel is nul
        return;
    }
    tick      = currentTick % (6 * parameters[LENGTH].value);
    int index = tick / 6; // beat number (16 beats by bar)
    if (tick % 6 == 0) { // good ppqn to play note on beat
        if (parameters[FREEZE].value == 0) { // not frozen
            notes[index] = getRandomNote();  // new random note
        }
        if (notes[index] < parameters[MIN].value ||
            parameters[MAX].value <= notes[index]) { // not in range
            return;
        }
        MIDI.sendNoteOn(notes[index], currentVelocity, 
            this->parameters[OUT].value);
        updatable = false; // don't change value (pitch and channel out)
    } else if (tick % 6 == parameters[GATE].value && !updatable) {
        MIDI.sendNoteOff(notes[index], 0, this->parameters[OUT].value);
        updatable = true; // now we can change value (pitch and channel out)
    }
}

/**
 * @brief Gestion des CCs.
 * 
 * Prise en charge pour la classe mère sauf pour la longueur et le gèle.
 */
void Random::handleControlChange(byte channel, byte number, byte value) {
#ifndef TEST
    if (number - 1 == FREEZE) {
        return;
    }
    if (number - 1 == LENGTH && parameters[FREEZE].value) {
        return;
    }
#endif
    Module::handleControlChange(channel, number, value);
}
