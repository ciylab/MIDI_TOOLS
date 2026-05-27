/**
 * @file Arp.cpp
 * @brief Gestion de l'arpégiateur.
 */

#include "Arp.h"

/**hauteur de la dernière note jouée utilisée pour note off */
byte last_pitch;

/**
 * @brief Rotation de l'encodeur VALUE
 *
 * Désactivé pour le jeu et l'enregistrement.
 */
void Arp::handleRotate1(int8_t rotation) {
    if (currentIndex == PLAY || currentIndex == RECORD) {
        return;
    }
    Module::handleRotate1(rotation);
}

/**
 * @brief Pression sur l'encodeur VALUE.
 *
 * Active/désactive le canal de sortie.
 * Inactif en dehors de PLAY et RECORD.
 * Initialise la séquence de jeu ou d'enregistrement.
 */
void Arp::handlePress1() {
    if (currentIndex == OUT) {
        if (parameters[currentIndex].value != 0) {
            parameters[currentIndex].buffer = 0;
            out = parameters[currentIndex].value;
        } else {
            parameters[currentIndex].buffer = out;
        }
        return;
    }
    if (currentIndex != PLAY && currentIndex != RECORD) {
        return;
    }
    if (currentIndex == PLAY && parameters[RECORD].value) {
        return;
    }
    if (currentIndex == RECORD && parameters[PLAY].value) {
        return;
    }
    parameters[currentIndex].buffer = 1 - parameters[currentIndex].value;
    if (parameters[PLAY].buffer) {
        seq.index = 0;
    } else if (parameters[RECORD].buffer) {
        seq.size = 0;
    }
}

/**
 * @brief Gestion de la note on.
 * 
 * Si c'est le bon canal d'entrée alors la note est jouée,
 * la mise à jour des données est gelée et la note est
 * potentiellement ajoutée à la séquence.
 *
 */

void Arp::handleNoteOn(byte channel, byte pitch, byte velocity) {
    byte out = parameters[OUT].value;
    if (out != 0 && channel == parameters[IN].value &&
            !parameters[PLAY].value) {
        if (parameters[RECORD].value) { // RECORD ON
            add(pitch, velocity);
            MIDI.sendNoteOn(pitch, velocity, out);
        } else if(seq.size == 0) {
            MIDI.sendNoteOn(pitch, velocity, out);
        } else {
            last_pitch = changePitch(seq.notes[seq.index].pitch) + 
                parameters[TRANSP].value;
            MIDI.sendNoteOn(last_pitch,
                    changeVelocity(seq.notes[seq.index].velocity), out);
            seq.index = (seq.index + 1) % seq.size;
        }
        updatable = false;
    }
}

/**
 * @brief Gestion de la note off.
 * 
 * Si c'est le bon canal d'entrée alors la note est arrêtée,
 * la mise à jour des données est possible.
 */

void Arp::handleNoteOff(byte channel, byte pitch, byte velocity) {
    byte out = parameters[OUT].value;
    if (!updatable) {
        if (parameters[RECORD].value) {
            MIDI.sendNoteOff(pitch, 0, out);
        } else if(seq.size == 0) {
            MIDI.sendNoteOff(pitch, 0, out);
        } else {
            MIDI.sendNoteOff(last_pitch, 0, out);
        }
        updatable = true;
    }
}

/**
 * @brief Gestion des CCs.
 * 
 * Prise en charge pour la classe mère sauf dans le cas de jeu ou
 * enregistrement.
 */

void Arp::handleControlChange(byte channel, byte number, byte value) {
    if (number - 1 == PLAY || number - 1 == RECORD) {
        return;
    }
    Module::handleControlChange(channel, number, value);
}

/**
 * @brief Gestion de l'horloge.
 *
 * Si on doit jouer alors sur le tick on joue la note, on gèle 
 * les éventuelles modifications et un gate après on arrête la note.
 * Le booléen aléatoire jump indique si la note est ignorée ou pas.
 * Le volume de la note est impacté par l'aléatoire et divisé par 2.
 */

void Arp::handleClock() {
    byte out = parameters[OUT].value;
    if (out == 0 || parameters[PLAY].value == 0 || seq.size == 0) {
        return;
    }
    tick = currentTick % (6 * seq.size);
    if (tick % 6 == 0 && !jump()) {
        seq.index = tick / 6;
        last_pitch = changePitch(seq.notes[seq.index].pitch) + 
            parameters[TRANSP].value;
        MIDI.sendNoteOn(last_pitch,
                        changeVelocity(seq.notes[seq.index].velocity), out);
        updatable = false;
    } else if (tick % 6 == parameters[GATE].value && !updatable) {
        MIDI.sendNoteOff(last_pitch, 0, out);
        updatable = true;
    }
}
