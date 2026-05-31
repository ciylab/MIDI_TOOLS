/**
 * @file midi.cpp
 * @brief Gestion des messages MIDI.
 */

#include "Display.h"
#include "Modules.h"
#include "encoder.h"
#include <Arduino.h> // micros()
#include <MIDI.h>

using namespace MIDI_NAMESPACE;

unsigned long currentTick = 0; /**< le temps est discrétisé en PPQN*/
unsigned long lastTime = micros(); /**<dernière mesure du temps*/
double bpm = 120; /**<bpm global*/
bool externalClock = false; /**<horloge interne par défaut*/
bool stopTick = false; /**<vrai si on met sur pause */
unsigned long delayBetweenTick = 2500000 / bpm; /**<temps entre les ticks*/
unsigned long previousBeat = micros(); /**<dernière noire */
extern bool showMenu;
extern int currentIndex;
extern MidiInterface<SerialMIDI<HardwareSerial>> MIDI; /**<interface MIDI*/
extern Modules ms;
extern Display oled;
extern int currentModule;
/**
 * @brief Gestion note on de la librairie
 * 
 * On renvoie à tous les modules
 */

void handleNoteOn(byte channel, byte pitch, byte velocity) {
    for (int i = 0; i < ms.size; i++) {
        ms.modules[i]->handleNoteOn(channel, pitch, velocity);
    }
}

/**
 * @brief Gestion note off de la librairie
 * 
 * On renvoie à tous les modules
 */


void handleNoteOff(byte channel, byte pitch, byte velocity) {
    for (int i = 0; i < ms.size; i++) {
        ms.modules[i]->handleNoteOff(channel, pitch, velocity);
    }
}

/**
 * @brief Gestion CC de la librairie.
 * 
 * Les numéros vont de 1 à 8.
 * On renvoie au module actif uniquement.
 */

void handleControlChange(byte channel, byte number, byte value) {
    if (number == 0 || 9 < number) {
        return;
    }
    if(showMenu) {
        return;
    }
    if (number - 1 < ms.modules[currentModule]->size) {
        ms.modules[currentModule]->handleControlChange(channel, number, value);
    }
}

/**
 * @brief Gestion PC de la librairie.
 * 
 * Les numéros vont de 0 à 7. 
 * On simule une manipulation manuelle. 
 */


void handleProgramChange(byte channel, byte number) {
    if (7 < number) {
        return;
    }
    showMenu = true;
    oled.putChar(8 * currentIndex, ' ');
    currentIndex = number % ms.size;
    handlePress0();
}

/**
 * @brief Gestion sysex de la librairie.
 *
 * On se contente de transmettre les données.
 */
void handleSystemExclusive(byte *array, unsigned int size) {
    MIDI.sendSysEx(size, array, true);
}

/**
 * @brief Gestion de l'horloge de la librairie.
 *
 * On renvoie la gestion à tous les modules.
 * Si l'horloge est externe et qu'on reçoit un signal, la pause est finie 
 * et on reprend le décompte des ticks.  
 */

void handleClock() {
    if(!externalClock) {
        return;
    }
    for (int i = 0; i < ms.size; i++) {
        ms.modules[i]->handleClock();
    }
    stopTick = false;
    if (currentTick == 0) {
        previousBeat = micros();
    } else if (currentTick % 24 == 0) {
        bpm = round(60000000. / (micros() - previousBeat));
        previousBeat = micros();
    }
    currentTick++;
    MIDI.sendRealTime(midi::Clock);
}

/**
 * @brief Gestion du temps hors librairie.
 *
 * Uniquement pour l'horloge interne. 
 * On marque le nouveau tick et on fait comme s'il venait 
 * d'une horloge externe. 
 */

void handleTime() {
   if (externalClock) {
        return;
    }
    if (micros() - lastTime >= delayBetweenTick) {
        lastTime += delayBetweenTick;
        for (int i = 0; i < ms.size; i++) {
            ms.modules[i]->handleClock();
        }
        if (!stopTick) {
            currentTick++;
        }
        MIDI.sendRealTime(midi::Clock);    
    }
}

/**
 * @brief Gestion start de la librairie.
 *
 * La commande start externe relance tout. 
 */

void handleStart() {
#ifndef TEST    
    if(!externalClock) {
        return;
    }
#endif
    currentTick = 0;
    stopTick    = false;
    lastTime    = micros();
    MIDI.sendRealTime(midi::Start);
}

/**
 * @brief Gestion stop de la librairie.
 *
 * La commande start de l'horloge externe arrête tout. 
 */

void handleStop() {
#ifndef TEST    
    if(!externalClock) {
        return;
    }
#endif
    stopTick = true;
    lastTime = micros();
    MIDI.sendRealTime(midi::Stop);
}
