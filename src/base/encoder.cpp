/**
 * @file encoder.cpp
 * @brief Comportement général des deux encodeurs.
 * 
 * Les appels sont antérieurs aux autres appels.
 *
 */

#include "encoder.h"
#include "Display.h"

extern Display oled;
extern Modules ms;
extern int currentIndex;
extern int currentModule;
extern bool showMenu;

bool newChoice = false; /**<vrai si on vient de changer le curseur */

/**
 * @brief Rotation de l'encodeur PARAMETER.
 *
 * Déplacement du curseur dans tous les cas
 * et affichage du paramètre précédent dans le cas d'un
 * algorithme à la place d'une éventuelle valeur. 
 *
 * Chaque rotation indique un nouveau choix.
 *
 */

void handleRotate0(int8_t rotation) {
    if(showMenu) {
        oled.showSelected(rotation, ms.size);
    } else {
        oled.showItem(ms.modules[currentModule]);
        oled.showSelected(rotation, ms.modules[currentModule]->size); 
    }
    newChoice = true;
}

/**
 * @brief Pression de l'encodeur PARAMETER.
 *
 * Alternance menu principal/algorithme.
 * Dans tous les cas, on rafraîchit toute la page.
 */


void handlePress0() {
    showMenu = !showMenu;
    if(showMenu) {
        strcpy(oled.buffer, ms.menu);
        oled.buffer[63] = '\0';
        oled.showSelected(0, ms.size);
    } else {
        currentModule = currentIndex;
        strcpy(oled.buffer, ms.modules[currentModule]->menu);
        oled.buffer[63] = '\0';
        oled.showSelected(0, ms.modules[currentModule]->size);
    }
    oled.charIndex = 1;
    oled.endPosition = 63;
    newChoice = true; // for the clock
}

/**
 * @brief Pression longue de l'encodeur PARAMETER.
 *
 * Reboot.
 */


void handleLongPress0() {
#ifdef NANOR4
    NVIC_SystemReset();
#else
    asm volatile("jmp 0x00");
#endif
}

/**
 * @brief Rotation de l'encodeur VALUE.
 *
 * Actif uniquement sur la page des algorithmes.
 * La première utilisation ne fait qu'afficher la valeur.
 * Les suivantes appellent la fonction du module. 
 */


void handleRotate1(int8_t rotation) {
    if(showMenu) {
        return;
    }
    if(!newChoice) {
        ms.modules[currentModule]->handleRotate1(rotation);
    } else {
        newChoice = false;
    }
    oled.showValue(ms.modules[currentModule]);
}

/**
 * @brief Pression de l'encodeur VALUE.
 *
 * Actif uniquement sur la page des algorithmes.
 * La première utilisation ne fait qu'afficher la valeur.
 * Les suivantes appellent la fonction du module.
 */

void handlePress1() {
    if(showMenu) {
        return;
    }
    if(!newChoice) {
        ms.modules[currentModule]->handlePress1();
    } else {
        newChoice = false;
    }
    oled.showValue(ms.modules[currentModule]);
}

/**
 * @brief Pression longue de l'encodeur VALUE.
 
 * Envoie de toutes les notes off sur la canal MIDI
 * dans le cas d'un algorithme.
 */


void handleLongPress1() {
    ms.modules[currentModule]->sendAllNoteOff();
}
