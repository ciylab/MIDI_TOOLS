/**
 * @file Looper.cpp
 * @brief Gestion du séquenceur.
 */

#include "Looper.h"

/**
 * @brief Pour stopper les notes jouées.
 */
void Looper::clean() {
    for (int i = 0; i < 128; i++) {
        if (playing[i]) {
            MIDI.sendNoteOff(i, 0, parameters[OUT].value);
            playing[i] = false;
        }
    }
}

/**
 * @brief Rotation de l'encodeur VALUE.
 *
 * Désactivé pour RECORD, UNDO et DEL(ETE). 
 */
void Looper::handleRotate1(int8_t rotation) {
    if (currentIndex == UNDO || currentIndex == RECORD ||
        currentIndex == DEL) {
        return;
    }
    Module::handleRotate1(rotation);
}

/**
 * @brief Pression de l'encodeur VALUE.
 *
 * Initialisation dans les trois options RECORD, UNDO et DEL(ETE)  
 */
void Looper::handlePress1() {
    if (currentIndex == OUT) {
        if (parameters[currentIndex].value != 0) {
            parameters[currentIndex].buffer = 0;
            clean();
            out = parameters[currentIndex].value;
        } else {
            parameters[currentIndex].buffer = out;
        }
        return;
    }
    if (currentIndex != UNDO && currentIndex != RECORD &&
        currentIndex != DEL) {
        return;
    }
    if (currentIndex == UNDO) {
        parameters[RECORD].buffer = 0;
        parameters[DEL].buffer    = 0;
    } else if (currentIndex == RECORD) {
        parameters[UNDO].buffer = 0;
        parameters[DEL].buffer  = 0;
    } else if (currentIndex == DEL) {
        parameters[UNDO].buffer   = 0;
        parameters[RECORD].buffer = 0;
    }
    parameters[currentIndex].buffer = 1 - parameters[currentIndex].value;
    if (parameters[UNDO].buffer) { // undoing
        if (0 < top_level && empty_recording) {
            top_level--;
            empty_recording = false;
        }
        if (0 < top_level) {
            top_level--;
            undo(top_level, levels, notes);
        }
        clean();
    } else if (parameters[RECORD].buffer) { // recording
        if (top_level < LEVEL_SIZE && !empty_recording) {
            top_level++;
        }
        empty_recording = true;
    } else if (parameters[DEL].buffer) { // deleting
        if (0 < top_level && empty_recording) {
            top_level--;
            empty_recording = false;
        }
        while (0 < top_level) {
            top_level--;
            undo(top_level, levels, notes);
        }
        empty_recording = false;
        clean();
    }
}

/**
 * @brief Gestion de l'horloge.
 *
 * Sur le tick on joue la note.
 */
void Looper::handleClock() {
    int length = LENGTH[parameters[LEN].value];
    if (length == 0) {
        return;
    }
    tick = currentTick % (6 * length);
    if (!parameters[UNDO].value && // not undoing
        !parameters[DEL].value) {  // not deleting
        play();
    }
}

/**
 * @brief Gestion de la note on.
 * 
 * On joue et on ajoute éventuellement la note sur la pile du tick.
 * On empile également le tick sur la pile du level.
 */
void Looper::handleNoteOn(byte channel, byte pitch, byte velocity) {
    byte out = parameters[OUT].value;
    if (out == 0) {
        return;
    }
    if (channel == parameters[IN].value) {
        MIDI.sendNoteOn(pitch, velocity, out);
        playing[pitch] = true;
        if (parameters[RECORD].value) { // recording
            empty_recording = false;
            push(&notes[tick], pitch, velocity, (byte)(top_level - 1),
                 true); // note on
            pushStackOfInt(&levels[top_level - 1], tick);
        }
    }
}

/**
 * @brief Gestion de la note off.
 * 
 * Même chose que pour la note on.
 *
 */
void Looper::handleNoteOff(byte channel, byte pitch, byte velocity) {
    byte out = parameters[OUT].value;
    if (out == 0) {
        return;
    }
    if (channel == parameters[IN].value) {
        MIDI.sendNoteOff(pitch, 0, out);
        playing[pitch] = false;
        if (parameters[RECORD].value) { // recording
            push(&notes[tick], pitch, velocity, (byte)(top_level - 1),
                 false); // note off
            pushStackOfInt(&levels[top_level - 1], tick);
        }
    }
}

/**
 * @brief Gestion des CCs.
 *
 * Prise en charge pour la classe mère sauf pour les paramètres
 * gérés par pression.
 */
void Looper::handleControlChange(byte channel, byte number, byte value) {
#ifndef TEST
    if (number - 1 == UNDO || number - 1 == RECORD || number - 1 == DEL) {
        return;
    }
#endif
    Module::handleControlChange(channel, number, value);
}

/**
 * @brief Jeu des notes.
 *
 * On joue toutes les notes sur la pile numéro tick.
 */
void Looper::play() {
    byte out = parameters[OUT].value;
    if (out == 0) {
        return;
    }
    noteStack *s = notes[tick];
    while (s != NULL) {
        if (s->on) {
            MIDI.sendNoteOn(s->pitch, s->velocity, out);
            playing[s->pitch] = true;
        } else {
            MIDI.sendNoteOff(s->pitch, 0, out);
            playing[s->pitch] = false;
        }
        s = s->next;
    }
}
