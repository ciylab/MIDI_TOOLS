/**
 * @file Sequencer.cpp
 * @brief Gestion du petit séquenceur.
 */

#include "Sequencer.h"
extern byte currentVelocity; /** voir Time.cpp */

/*
 * @brief Gestion de l'horloge.
 *
 * Sur le tick on joue la note sauf si elle est désactivée.
 */
void Sequencer::handleClock() {
    byte out = parameters[OUT].value;
    if (out == 0 || parameters[LEN].value == 0) {
        return;
    }
    byte l = 6 * parameters[SPEED].value;
    tick   = currentTick % (l * parameters[LEN].value);
    index  = tick / l;
    if (tick % l == 0 && notes[index]) {
        MIDI.sendNoteOn(parameters[index + 2].value, currentVelocity, out);
        updatable = false;
    } else if (tick % l == GATE && !updatable) {
        MIDI.sendNoteOff(parameters[index + 2].value, 0, out);
        updatable = true;
    }
}

/**
 * @brief Rotation de l'encodeur VALUE
 *
 * Désactivé pour le jeu et l'enregistrement.
 * */
void Sequencer::handleRotate1(int8_t rotation) {
    if (1 < currentIndex && currentIndex < 7 && 
            !notes[currentIndex - 2]) {
        return;
    }
    Module::handleRotate1(rotation);
}

/**
 * @brief Pression sur l'encodeur VALUE.
 *
 * Active/désactive le canal de sortie et les notes mais
 * inactif pour la vitesse de lecture.
 */
void Sequencer::handlePress1() {
    if (currentIndex == OUT) {
        if (parameters[currentIndex].value != 0) {
            parameters[currentIndex].buffer = 0;
            out = parameters[currentIndex].value;
        } else {
            parameters[currentIndex].buffer = out;
        }
        return;
    }
    if (currentIndex == SPEED) {
        return;
    }
    if (1 < currentIndex && currentIndex < 7) {
        notes[currentIndex - 2] = !notes[currentIndex - 2];
    }
}
