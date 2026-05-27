/**
 * @file Compare.cpp
 * @brief Gestion du comparateur.
 */

#include "Compare.h"
/**
 * @brief Rotation de l'encodeur VALUE
 * */

void Compare::handleRotate1(int8_t rotation) {
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
            parameters[currentIndex].buffer < parameters[currentIndex].max) {
            parameters[currentIndex].buffer++;
        } else if (rotation < 0 &&
                   parameters[MIN].value < parameters[currentIndex].buffer) {
            parameters[currentIndex].buffer--;
        }
        break;
    default:
        Module::handleRotate1(rotation);
        break;
    }
}

/**
 * @brief Pression de l'encodeur VALUE.
 *
 * Uniquement pour activer/désactiver le canal de sortie.
 */

void Compare::handlePress1() {
    if (currentIndex == OUT) {
        if (parameters[currentIndex].value != 0) {
            parameters[currentIndex].buffer = 0;
            out = parameters[currentIndex].value;
        } else {
            parameters[currentIndex].buffer = out;
        }
        return;
    }
}

/**
 * @brief Gestion de la note on.
 * 
 * La note est jouée quand elle est dans l'intervalle. 
 * On ajoute éventuellement une transposition.
 */

void Compare::handleNoteOn(byte channel, byte pitch, byte velocity) {
    byte out = parameters[OUT].value;
    if (out == 0) {
        return;
    }
    if (channel == parameters[IN].value) {
        if (parameters[MIN].value <= pitch && pitch < parameters[MAX].value) {
            MIDI.sendNoteOn(pitch + parameters[TRANSP].value, velocity, out);
            updatable = false;
        }
    }
}

/**
 * @brief Gestion de la note off.
 * 
 * La note est arrêtée (quand elle est dans l'intervalle). 
 * On ajoute éventuellement une transposition.
 */

void Compare::handleNoteOff(byte channel, byte pitch, byte velocity) {
    byte out = parameters[OUT].value;
    if (out == 0) {
        return;
    }
    if (channel == parameters[IN].value) {
        if (parameters[MIN].value <= pitch && pitch < parameters[MAX].value) {
            MIDI.sendNoteOff(pitch + parameters[TRANSP].value, 0, out);
            updatable = true;
        }
    }
}
