/**
 * @file Random.h
 * @brief Fonctions du générateur aléatoire.
 */

#ifndef RANDOM_H
#define RANDOM_H
#include "../base/Module.h"

#define R_SEQ_SIZE 64 /**<taille de la mémoire pour enregistrer 4 mesures de 16 double-croches */
#define NUM_SCALE 4 /**<nombre de gammes */

class Random : public Module {
  private:
    byte notes[R_SEQ_SIZE];
    byte out;
    const char *ONOFF[2]         = {"OFF", " ON"};
    const char *NOTES[12]        = {"C",  "C#", "D",  "D#", "E",  "F",
                                    "F#", "G",  "G#", "A",  "A#", "B"};
    const char *str_scales[NUM_SCALE] = 
        {"CHROMA", "MAJOR", "PENTA", "HARMO"};
    static int scale_size[NUM_SCALE];
    static byte count[NUM_SCALE][12];  // number of pitch lower than the one
    static byte pitchs[NUM_SCALE][12]; // i-th pitch in C
    static byte scales[NUM_SCALE][12];
    enum param { LENGTH, OUT, GATE, FREEZE, MIN, MAX, KEY, SCALE, END_ };

  public:
    /**
     * @brief Constructeur par défaut.
     */
    Random(char *name) : Module(name) {
        strcpy(parameters[LENGTH].name, "LENGTH");
        parameters[LENGTH].max = 64;
        strcpy(parameters[OUT].name, "OUT");
        parameters[OUT].max = 16;
        strcpy(parameters[GATE].name, "GATE");
        parameters[GATE].value = parameters[GATE].buffer = 3;
        parameters[GATE].min                             = 1;
        parameters[GATE].max                             = 5;
        strcpy(parameters[FREEZE].name, "FREEZE");
        parameters[FREEZE].max = 1;
        strcpy(parameters[MIN].name, "MIN");
        parameters[MIN].value = parameters[MIN].buffer = MIDDLE_C - 12;
        parameters[MIN].min                            = NOTE_MIN;
        parameters[MIN].max                            = NOTE_MAX;
        strcpy(parameters[MAX].name, "MAX");
        parameters[MAX].value = parameters[MAX].buffer = MIDDLE_C + 12;
        parameters[MAX].min                            = NOTE_MIN;
        parameters[MAX].max                            = NOTE_MAX;
        strcpy(parameters[KEY].name, "KEY");
        parameters[KEY].max = 11;
        strcpy(parameters[SCALE].name, "SCALE");
        parameters[SCALE].max = NUM_SCALE - 1;
        this->size            = END_; // number of parameters
        for (int i = 0; i < R_SEQ_SIZE; i++) {
            this->notes[i] = 0;
        }
        this->updatable = true;
        this->setMenu();
        this->init_arrays();
        this->out = parameters[OUT].value;
    }

    byte getOut() {
        return parameters[OUT].value;
    }

    void getString(char str[8]) {
        switch (currentIndex) {
        case GATE:
            setProgressString5(str, parameters[currentIndex].buffer);
            break;
        case OUT:
            if (parameters[currentIndex].buffer == 0) {
                sprintf(str, "%s", "OFF");
            } else {
                sprintf(str, "%2d", parameters[currentIndex].buffer);
            }
            break;
        case LENGTH:
            sprintf(str, "%2d", parameters[currentIndex].buffer);
            break;
        case FREEZE:
            sprintf(str, "%s", ONOFF[parameters[currentIndex].buffer]);
            break;
        case MIN:
            sprintf(str, "%2s%d", 
                    NOTES[parameters[currentIndex].buffer % 12],
                    parameters[currentIndex].buffer / 12 - 1);
            break;
        case MAX:
            sprintf(str, "%2s%d", 
                    NOTES[parameters[currentIndex].buffer % 12],
                    parameters[currentIndex].buffer / 12 - 1);
            break;
        case KEY:
            sprintf(str, "%s", NOTES[parameters[currentIndex].buffer]);
            break;
        case SCALE:
            sprintf(str, "%s", 
                    str_scales[parameters[currentIndex].buffer]);
            break;
        default:
            break;
        }
    }
/**
 * @brief Simple fonction retournant une note aléatoire dans le contexte.
 *
 * Utilisée par l'horloge.
 */
    byte getRandomNote() {
        return rand_note(parameters[MIN].value, parameters[MAX].value,
                         parameters[KEY].value, parameters[SCALE].value);
    }

    void init_arrays();
    byte get_count(byte scale, byte note);
    byte get_pitch(byte scale, byte rank);
    byte rand_note(byte min, byte max, byte tone, byte scale);
    void handleControlChange(byte channel, byte number, byte value);
    void handleNoteOn(byte channel, byte pitch, byte velocity) {}
    void handleNoteOff(byte channel, byte pitch, byte velocity) {}
    void handleClock();
    void handleRotate1(int8_t rotation);
    void handlePress1();
};

#endif
