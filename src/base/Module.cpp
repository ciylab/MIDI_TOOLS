/**
 * @file Module.cpp
 * @brief Gestion d'un module.
 *
 * Un module est un algorithme qui traite les données MIDI
 * et gère l'encodeur VALUE
 */

#include "Module.h"
#include "Display.h"
#include <string.h>

using namespace MIDI_NAMESPACE;
extern MidiInterface<SerialMIDI<HardwareSerial>> MIDI;
extern int currentIndex;
extern int currentModule;
extern Display oled;
extern Modules ms;

/**
 * @brief Constructeur.
 */

Module::Module() {}

/**
 * @brief Constructeur avec initialisation des paramètres.
 */

Module::Module(char *name) {
    strcpy(this->name, name);
    tick = 0;
    for (int i = 0; i < 8; i++) {
        parameters[i].value  = 0;
        parameters[i].buffer = 0;
        parameters[i].min    = 0;
        parameters[i].max    = 0;
        parameters[i].cc     = i + 1;
    }
    for (int i = 0; i < 63; i++) {
        menu[i] = ' ';
    }
    menu[63] = '\0';
}

/**
 * @brief Menu spécifique au module.
 */

void Module::setMenu() {
    for (int j = 0; j < size; j++) {
        int i = 0;
        while (parameters[j].name[i] != '\0') {
            menu[i + 8 * j + 1] = parameters[j].name[i];
            i++;
        }
    }
}

/**
 * @brief Gestion des CC pour le module.
 *
 * Sélection du paramètre puis modification du tampon 
 * des données et mise à jour de l'écran.
 */

void Module::handleControlChange(byte channel, byte number, byte value) {
    if(currentIndex != number - 1) {
        oled.showItem(ms.modules[currentModule]);
        oled.putChar(8 * currentIndex, ' ');
        currentIndex = number - 1;
        oled.putChar(8 * currentIndex, CURSOR);
    } else {
        parameters[currentIndex].buffer =
            map(value, 0, 127, parameters[currentIndex].min,
                parameters[currentIndex].max);
        oled.showValue(ms.modules[currentModule]);
    }
}

/**
 * @brief Modification du tampon des données.
 */

void Module::handleRotate1(int8_t rotation) {
    if (rotation > 0 &&
        parameters[currentIndex].buffer < parameters[currentIndex].max) {
        parameters[currentIndex].buffer++;
    } else if (rotation < 0 && parameters[currentIndex].min <
                                   parameters[currentIndex].buffer) {
        parameters[currentIndex].buffer--;
    }
}

/**
 * @brief Mise à jour des données.
 *
 * Si le moment le permet, les valeurs du paramètre
 * sont mises à jour. Un blocage peut être par exemple
 * le changement de canal MIDI avant une note off.
 */

void Module::update(int i) {
    if (!updatable) {
        return;
    }
    if (parameters[i].value == parameters[i].buffer) {
        return;
    }
    parameters[i].value = parameters[i].buffer;
}

/**
 * @brief Mise à jour des données.
 *
 * Toutes les données sont mises à jour.
 */

void Module::update() {
    for (int i = 0; i < this->size; i++) {
        update(i);
    }
}

/**
 * @brief MIDI panic.
 *
 * Si le canal de sortie n'est pas nul, cette fonction envoie toutes les
 * notes off.
 */

void Module::sendAllNoteOff() {
    byte out = getOut();
    if (out == 0) {
        return;
    }
    for (int i = 0; i < 127; i++) {
        MIDI.sendNoteOff(i, 0, out);
    }
}

