/**
 * @file Delay.cpp
 * @brief Gestion de l'écho.
 */

#include "Delay.h"

extern double bpm;

/**
 * @brief Retourne le temps entre deux échos.
 */

double Delay::getDelayTime() {
    return parameters[FINE].value - 10 +
        sync[this->parameters[TIME].value] / bpm;
}

/**
 * @brief Gestion de l'écho.
 *
 * On regarde la note au début de la file, la première entrée. 
 * On la joue s'il est temps de la jouer et on la rajoute en fin de
 * file en diminuant la répétition de 1. 
 */

void Delay::handle() {
    byte out = this->parameters[OUT].value;
    if (out == 0) {
        return;
    }
    note n = this->buf.notes[this->buf.queue];
    unsigned long now = millis();
    if (n.p_time != 0 && now >= n.p_time + this->getDelayTime()) {
        if (n.noteOn) {
            MIDI.sendNoteOn(n.pitch, n.velocity, out);
            updatable = false;
        } else {
            MIDI.sendNoteOff(n.pitch, 0, out);
            updatable = true;
        }
        if (n.feedback > 0) {
            add(changePitch(n.pitch), changeVelocity(n.velocity), 
                    now, n.noteOn, n.feedback - 1);
        }
        this->buf.notes[this->buf.queue].p_time = 0;
        this->buf.queue = (this->buf.queue + 1) % BUF_SIZE;
    }
}

/**
 * @brief Gestion de la note on.
 * 
 * Si c'est le bon canal d'entrée alors la note est jouée,
 * puis ajoutée à la fin de la queue.
 */

void Delay::handleNoteOn(byte channel, byte pitch, byte velocity) {
    byte out = this->parameters[OUT].value;
    if (out != 0 && channel == this->parameters[IN].value) {
        MIDI.sendNoteOn(pitch, velocity, out);
        updatable = false;
        if (0 < this->parameters[FBK].value) {
            add(changePitch(pitch), changeVelocity(velocity), millis(), 
                    true, this->parameters[FBK].value - 1);
        }
    }
}

/**
 * @brief Gestion de la note off.
 * 
 * Même principe que note on.
 */

void Delay::handleNoteOff(byte channel, byte pitch, byte velocity) {
    byte out = this->parameters[OUT].value;
    if (out != 0 && channel == this->parameters[IN].value) {
        MIDI.sendNoteOff(pitch, 0, out);
        updatable = true;
        if (0 < this->parameters[FBK].value) {
            add(changePitch(pitch), changeVelocity(velocity), millis(), 
                    false, this->parameters[FBK].value - 1);
        }
    }
}

/**
 * @brief Pression de l'encodeur VALUE.
 *
 * Uniquement pour activer/désactiver le canal de sortie.
 * */

void Delay::handlePress1() {
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
