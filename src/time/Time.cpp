/**
 * @file Time.cpp
 * @brief Gestion du temps.
 */
 
#include "Time.h"
#include "../base/Display.h"
#include "../base/midi.h"

extern bool showMenu;
extern bool externalClock;
extern unsigned long delayBetweenTick;
extern double bpm;
extern Display oled;
byte currentVelocity = DEFAULT_VELOCITY;

/**
 * @brief Rotation de l'encodeur VALUE
 *
 * Désactivé pour le jeu et l'enregistrement.
 * */
void Time::handleRotate1(int8_t rotation) {
    if (currentIndex == CLCK || currentIndex == METRONOME) {
        return;
    }
    if (currentIndex == BPM && externalClock) {
        return;
    }
    Module::handleRotate1(rotation);
}

/**
 * @brief Pression sur l'encodeur VALUE.
 *
 * Inactif en dehors de la source d'horloge et du métronome.
 */
void Time::handlePress1() {
    if (currentIndex == OUT) {
        if (parameters[currentIndex].value != 0) {
            parameters[currentIndex].buffer = 0;
            out = parameters[currentIndex].value;
        } else {
            parameters[currentIndex].buffer = out;
        }
        return;
    }
    if (currentIndex != CLCK && currentIndex != METRONOME) {
        return;
    }
    parameters[currentIndex].buffer = 1 - parameters[currentIndex].value;
}

/**
 * @brief Mise à jour des paramètres.
 *
 * Cas particulier de l'affichage du bpm. 
 */
void Time::update(int i) {
    if (!updatable) {
        return;
    }
    if (i == BPM && externalClock) {
        parameters[i].buffer = bpm;
    }
    if (parameters[i].value == parameters[i].buffer) {
        return;
    }
    switch (i) {
    case CLCK:
        parameters[i].value = parameters[i].buffer;
        if (!parameters[CLCK].value) { // externalClock = false
            externalClock = true; 
            handleStart();
            externalClock = false;
        } else {
            externalClock = true;
            handleStop();
        }
        break;
    case BPM:
        if (!externalClock) {
            bpm = parameters[i].buffer;
            delayBetweenTick = 2500000 / bpm;
        } else if (i == currentIndex) {
            oled.showValue(this);
        }
        parameters[i].value = parameters[i].buffer;
        break;
    case VELOCITY:
        parameters[i].value = parameters[i].buffer;
        currentVelocity = parameters[i].value;
        break;
    default:
        Module::update(i);
        break;
    }
}

/**
 * @brief Gestion de l'horloge.
 *
 * Commande de la led et des notes jouées par le métronome.
 */
void Time::handleClock() {  
    // 
    switch (currentTick % 24) {
    case 0:
        digitalWrite(LED_CLOCK, HIGH);
        break;
    case 1:
        digitalWrite(LED_CLOCK, LOW);
        break;
    default:
        break;
    }    
    byte out = parameters[OUT].value;
    if (out == 0) {
        return;
    }    if (parameters[METER].value == 0) {
        return;
    }
    tick = currentTick % (24 * parameters[METER].value);
    //     
    if (!parameters[METRONOME].value) {
        return;
    }
    switch (tick % 24) {
    case 0:
        if (tick == 0) {
            MIDI.sendNoteOn(parameters[FIRST].value, 
                parameters[VELOCITY].value, out);
        } else {
            MIDI.sendNoteOn(parameters[OTHER].value, 
                parameters[VELOCITY].value, out);
        }
        updatable = false;
        break;
    case 3:
        if (tick == 3) {
            MIDI.sendNoteOff(parameters[FIRST].value, 0, out);
        } else {
            MIDI.sendNoteOff(parameters[OTHER].value, 0, out);
        }
        updatable = true;
        break;
    default:
        break;
    }
}

/**
 * @brief Gestion des CCs.
 *
 * Inactif quand le paramètre est géré par pression.
 */
void Time::handleControlChange(byte channel, byte number, byte value) {
#ifndef TEST
    if(number - 1 == CLCK || number - 1 == METRONOME) {
        return;
    }
#endif
    if (number - 1 == BPM && externalClock) {
        return;
    }
    Module::handleControlChange(channel, number, value);
} 
