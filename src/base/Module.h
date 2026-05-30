/**
 * @file Module.h
 * @brief Déclaration de la classe.
 *
 * La plage de notes est celle d'un clavier de 88 touches.
 */

#ifndef MODULE_H
#define MODULE_H
#include <Arduino.h>
#include <MIDI.h>

#define TEST

using namespace MIDI_NAMESPACE;
extern MidiInterface<SerialMIDI<HardwareSerial>> MIDI; /**<interface MIDI */
extern int currentIndex;
extern unsigned long currentTick;

#define MIDDLE_C 60 /**< C4 */
#define NOTE_MIN 21 /**< A0 */
#define NOTE_MAX 108 /**< C8 */
#define BPM_MIN 30 /**<min bpm of the module */
#define BPM_MAX 240 /**<max bpm of the module */
#define BPM_DEFAULT 120 /**< bpm moyen en club*/
#define CURSOR '>' /**< curseur de l'écran*/
#define DEFAULT_VELOCITY 100 /** vélocité par défaut*/

/** 
 * @brief Tableau des chaînes pour les barres de progression indiquant les %
 * d'aléatoire.
 *
 * Le caractère dollar est remplacé par un demi-rectangle plein 
 * et l'apostrophe par un rectangle plein.
 *
 */

class Module {
  private:
    typedef struct parameter {
        char name[8]; // name to display
        byte value;   // uses when playing
        byte buffer;  // value before update to prevent change when note on
        byte min;     // min value
        byte max;     // max value
        byte cc;      // cc number
    } parameter;

  public:
    byte out; /**<canal de sortie */
    int tick; /**<tick pour ce module */
    boolean updatable; /**<vrai si on peut changer les valeurs */
    parameter parameters[8]; /**<tableau des paramètres */
    int size; /**<nombre de paramètres */
    char name[8]; /**<nom du module sur l'écran */
    char menu[64]; /**< texte à afficher */
    Module();      // do nothing but needed
    Module(char *);
    virtual void getString(char s[8]) = 0; /**<Constructeur du menu.*/
    virtual void handleClock() = 0;
    
    /**
     * @brief Copie de la chaîne indiquant la barre de progression.
     * Le caractère dollar est remplacé par un demi-rectangle plein 
     * et l'apostrophe par un rectangle plein.
     */
    void setProgressString(char str[8], byte value) {
        int i = 0;
        if(value != 0){
            while(i <= (value - 1) / 2) {
                str[i] = '\'';
                i++;
            }
            if (value % 2 == 1) {
                str[i - 1] = '$';
            }
        }
        while(i < 7) {
            str[i] = ' ';
            i++;
        }
        str[i] = '\0';
    }

    void setProgressString5(char str[8], byte value) {
        int i = 0;
        while(i < value) {
            str[i] = '\'';
            i++;
        }
        while(i < 7) {
            str[i] = ' ';
            i++;
        }
        str[i] = '\0';
    }
    
    virtual void handleNoteOn(byte channel, byte pitch, byte velocity)  = 0;
    virtual void handleNoteOff(byte channel, byte pitch, byte velocity) = 0;
    virtual void handleRotate1(int8_t rotation);
    virtual void handlePress1() = 0;
    void sendAllNoteOff();
    virtual void update(int);
    virtual byte getOut() = 0; /**<Retourne le canal de sortie */
    virtual void handleControlChange(byte channel, byte number, byte value);
    void update();
    void setMenu();
};

#endif
