/**
 * @file Euclidean.cpp
 * @brief Gestion du générateur de séquences euclidiennes.
 */

#include "Euclidean.h"
#include "../base/Display.h"
#include "../base/midi.h"

extern byte currentVelocity; /** Mis à jour dans Time.cpp */

/**
 * @brief Vérification des contraintes du nombre de pas.
 */
void Euclidean::check_constraints() {
    for(int i = 4; i < 8; i++) {
        parameters[i].max = LENGTH[parameters[LEN].buffer];
        if (parameters[i].max < parameters[i].value) {
            parameters[i].buffer = parameters[i].max;
        }
    }
}

/**
 * @brief Rotation de l'encodeur VALUE.
 *
 * Avec gestion des contraintes comme BEATS < LENGTH.
 */
void Euclidean::handleRotate1(int8_t rotation) {
    if(currentIndex == LEN) {
        if (rotation > 0 &&
            parameters[currentIndex].buffer < 
                parameters[currentIndex].max) {
            parameters[currentIndex].buffer++;
        } else if (rotation < 0 && 0 < parameters[currentIndex].buffer) {
            parameters[currentIndex].buffer--;
        }
        check_constraints();
    } else {
        Module::handleRotate1(rotation);
    }
}

/**
 * @brief Pression de l'encodeur VALUE.
 *
 * Activation/désactivation des sorties.
 */

void Euclidean::handlePress1() {
    if (currentIndex == OUT) {
        if (parameters[currentIndex].value != 0) {
            parameters[currentIndex].buffer = 0;
            out = parameters[currentIndex].value;
        } else {
            parameters[currentIndex].buffer = out;
        }
        return;
    } else if (currentIndex == BEATS1) {
        if (parameters[BEATS1].value != 0) {
            parameters[BEATS1].buffer = 0;
            ticks1 = parameters[BEATS1].value;
        } else {
            parameters[BEATS1].buffer = ticks1;
        }

    } else if (currentIndex == BEATS2) {
        if (parameters[BEATS2].value != 0) {
            parameters[BEATS2].buffer = 0;
            ticks2 = parameters[BEATS2].value;
        } else {
            parameters[BEATS2].buffer = ticks2;
        }
    }
}

/**
 * @brief Gestion de l'horloge pour envoyer les notes.
 */
void Euclidean::handleClock() {
    int length = LENGTH[parameters[LEN].value];
    byte out   = parameters[OUT].value;
    if (out == 0 || length == 0) {
        return;
    }
    this->tick = currentTick % (6 * length);
    if (tick % 6 == 0) {
        if ((parameters[BEATS1].value *
             (length - parameters[SHIFT1].value + tick / 6)) % length <
                parameters[BEATS1].value) {
            pitch = parameters[NOTE1].value;
            MIDI.sendNoteOn(pitch, currentVelocity, out);
            updatable = false;
        } else if ((parameters[BEATS2].value *
                (length - parameters[SHIFT2].value + tick / 6)) % length <
                parameters[BEATS2].value) {
            pitch = parameters[NOTE2].value;
            MIDI.sendNoteOn(pitch, currentVelocity, out);
            updatable = false;
        }
    } else if (tick % 6 == 3 && !updatable) {
        MIDI.sendNoteOff(pitch, 0, out);
        updatable = true;
    }
}

/**
 * @brief Gestion des CCs.
 * 
 * Prise en compte de la longueur pour le nombre de pas et le décalage.
 */
void Euclidean::handleControlChange(byte channel, byte number, byte value) {
    Module::handleControlChange(channel, number, value);
    if (number - 1 == LEN) {
        check_constraints();
    }
}
