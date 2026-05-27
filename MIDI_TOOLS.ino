/**
 * @file MIDI_tools.ino
 * @brief Fichier principal du projet 
 * 
 * Le module reçoit des données MIDI et grâce à des algorithmes
 * stockés dans le micro-contrôleur et sélectionnés par les encodeurs
 * renvoie d'autres données MIDI.
 * @author Pierrick MEIGNEN 
 * @version 1.0
 * @date 02/01/2025 
 */
#include <MIDI.h>
#include <U8x8lib.h>
#include <Versatile_RotaryEncoder.h>

/*** base ***/
#include "src/base/Display.h"
#include "src/base/Modules.h"
#include "src/base/encoder.h"
#include "src/base/midi.h"
/*** modules ***/
#include "src/arpeggiator/Arp.h"
#include "src/compare/Compare.h"
#include "src/delay/Delay.h"
#include "src/looper/Looper.h"
#include "src/random/Random.h"
#include "src/euclidean/Euclidean.h"
#include "src/sequencer/Sequencer.h"
#include "src/time/Time.h"

Modules ms; /**<liste des modules */
Display oled; /**<l'écran */
/**le tableau des deux encodeurs */
Versatile_RotaryEncoder *versatile_encoder[NUM_ENC];
/**instance MIDI*/

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

int sws[NUM_ENC]  = {10, 20}; /**<pin des switchs */
int dts[NUM_ENC]  = {11, 21}; /**<pin des datas */
int clks[NUM_ENC] = {12, 17}; /**<pin des clocks */

int currentIndex; /**<l'indice du curseur compris entre 0 et 7 */
int currentModule; /**<l'indice du module actif */
bool showMenu; /**<vrai si le menu principal est affiché */

/************* modules ******************/
/**déclaration du module de comparaison */
Compare *cmp         = new Compare((char *)"COMPAR");
/**déclaration du module d'écho */
Delay *dl            = new Delay((char *)"DELAY");
/**déclaration du module de gestion du temps */
Time *timer          = new Time((char *)"TIME");
/**déclaration du module de séquence */ 
Sequencer *seq       = new Sequencer((char *)"5-SEQ");
/**déclaration du module d'arpégiateur */ 
Arp *arppegiator     = new Arp((char *)"ARPEGG");
/**déclaration du module de loop */
Looper *looper       = new Looper((char *)"LOOPER");
/**déclaration du module des deux séquences euclidiennes */
Euclidean *euclidean = new Euclidean((char *)"EUCLID");
/**déclaration du module de génération aléatoire */
Random *rm           = new Random((char *)"RANDOM"); 

/**
* @brief Initialisation
*
* - fonte
* - ajout des modules
* - déclaration des encodeurs
* - déclaration de la gestion MIDI
* - affichage du message de bienvenue pendant 3 secondes
* - affichage du menu principal
*/

void setup() {
    oled.begin();
    /************* modules ******************/
    ms.add(timer);
    ms.add(seq);
    ms.add(arppegiator);
    ms.add(cmp);
    ms.add(dl);
    ms.add(looper);
    euclidean->setDrum1(MIDDLE_C - 12, 4, 0);
    euclidean->setDrum2(MIDDLE_C + 12, 4, 2);
    ms.add(euclidean);
    ms.add(rm);
    /****************************************/
    pinMode(LED_CLOCK, OUTPUT);
    for (int i = 0; i < NUM_ENC; i++) {
        pinMode(clks[i], INPUT_PULLUP);
        pinMode(dts[i], INPUT_PULLUP);
        pinMode(sws[i], INPUT_PULLUP);
    }
    for (int i = 0; i < NUM_ENC; i++) {
        versatile_encoder[i] =
            new Versatile_RotaryEncoder(clks[i], dts[i], sws[i]);
    }
    currentIndex = 0;
    currentModule = 0;
    versatile_encoder[0]->setHandleRotate(handleRotate0);    
    versatile_encoder[0]->setHandlePress(handlePress0);
    versatile_encoder[0]->setHandleLongPress(handleLongPress0);
    versatile_encoder[1]->setHandleRotate(handleRotate1);
    versatile_encoder[1]->setHandlePress(handlePress1);
    versatile_encoder[1]->setHandleLongPress(handleLongPress1);
    MIDI.begin(MIDI_CHANNEL_OMNI);
    MIDI.turnThruOff();
    MIDI.setHandleClock(handleClock);
    MIDI.setHandleStart(handleStart);
    MIDI.setHandleStop(handleStop);
    MIDI.setHandleNoteOn(handleNoteOn);   // Listen to note on
    MIDI.setHandleNoteOff(handleNoteOff); // Listen to note off
    MIDI.setHandleControlChange(handleControlChange);
    MIDI.setHandleProgramChange(handleProgramChange);
    MIDI.setHandleSystemExclusive(handleSystemExclusive);
    oled.welcome();
    delay(2000);
    showMenu = false;
    handlePress0();
}

/**
 * @brief La boucle principale
 *
 * - lecture des données MIDI
 * - lecture des données des encodeurs
 * - mise à jour de l'affichage
 * - gestion de l'horloge interne
 * - mise à jour des paramètres
 * - gestion de l'écho
 */

void loop() {
    MIDI.read();
    for (int i = 0; i < NUM_ENC; i++) {
        versatile_encoder[i]->ReadEncoder();
    }
    oled.display();
    handleTime();
    if(!showMenu) {
        ms.modules[currentModule]->update();
    }
    dl->handle();
}

