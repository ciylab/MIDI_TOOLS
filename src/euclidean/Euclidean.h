/**
 * @file Euclidean.h
 * @brief Fonctions du généréteur de séquences euclidiennes.
 */

#ifndef EUCLIDEAN_H
#define EUCLIDEAN_H
#include "../base/Module.h"

class Euclidean : public Module {
  private:
    const char *NOTES[12] = {" C", "C#", " D", "D#", " E", "F",
                             "F#", " G", "G#", " A", "A#", " B"};
    const int LENGTH[9]   = {3, 4, 5, 6, 8, 12, 16, 24, 32};
    byte pitch;
    enum param { LEN, OUT, NOTE1, NOTE2, BEATS1, BEATS2, SHIFT1, SHIFT2, END_ };
    byte ticks1;
    byte ticks2;

  public:
/**
 * @brief constructeur par défaut
 */
    Euclidean(char *name) : Module(name) {
        strcpy(this->parameters[LEN].name, "LENGTH");
        parameters[LEN].value = parameters[LEN].buffer = 6;
        parameters[LEN].max                            = 8;
        strcpy(this->parameters[OUT].name, "OUT");
        parameters[OUT].max = 16;
        strcpy(this->parameters[NOTE1].name, "NOTE1");
        parameters[NOTE1].value = parameters[NOTE1].buffer = MIDDLE_C;
        parameters[NOTE1].min                              = NOTE_MIN;
        parameters[NOTE1].max                              = NOTE_MAX;
        strcpy(this->parameters[NOTE2].name, "NOTE2");
        parameters[NOTE2].value = parameters[NOTE2].buffer = MIDDLE_C;
        parameters[NOTE2].min                              = NOTE_MIN;
        parameters[NOTE2].max                              = NOTE_MAX;
        strcpy(this->parameters[BEATS1].name, "BEATS1");
        parameters[BEATS1].max = 16;
        strcpy(this->parameters[BEATS2].name, "BEATS2");
        parameters[BEATS2].max = 16;
        strcpy(this->parameters[SHIFT1].name, "SHIFT1");
        parameters[SHIFT1].max = 16;
        strcpy(this->parameters[SHIFT2].name, "SHIFT2");
        parameters[SHIFT2].max = 16;
        this->size             = END_; // number of parameters
        this->updatable        = true;
        this->setMenu();
        this->ticks1 = parameters[BEATS1].value;
        this->ticks2 = parameters[BEATS2].value;
        this->out    = parameters[OUT].value;
    }

    /**
     * @brief affectation des trois paramètres.
     */
    void setDrum1(byte tone, byte ticks, byte shift) {
        this->parameters[NOTE1].buffer  = tone;
        this->parameters[BEATS1].buffer = ticks;
        this->parameters[SHIFT1].buffer = shift;
    }

    /**
     * @brief affectation des trois paramètres.
     */
    void setDrum2(byte tone, byte ticks, byte shift) {
        this->parameters[NOTE2].buffer  = tone;
        this->parameters[BEATS2].buffer = ticks;
        this->parameters[SHIFT2].buffer = shift;
    }

    byte getOut() {
        return parameters[OUT].value;
    }

    void getString(char str[8]) {
        switch (currentIndex) {
        case LEN:
            sprintf(str, "%2d", LENGTH[parameters[currentIndex].buffer]);
            break;
        case OUT:
            if (parameters[currentIndex].buffer == 0) {
                sprintf(str, "%s", "OFF");
            } else {
                sprintf(str, "%2d", parameters[currentIndex].buffer);
            }
            break;
        case NOTE1:
            sprintf(str, "%2s%d", 
                    NOTES[parameters[currentIndex].buffer % 12],
                    parameters[currentIndex].buffer / 12 - 1);
            break;
        case NOTE2:
            sprintf(str, "%2s%d", 
                    NOTES[parameters[currentIndex].buffer % 12],
                    parameters[currentIndex].buffer / 12 - 1);
            break;
        default:
            sprintf(str, "%2d", parameters[currentIndex].buffer);
            break;
        }
    }
    void handleNoteOn(byte channel, byte pitch, byte velocity) {}
    void handleNoteOff(byte channel, byte pitch, byte velocity) {}
    void handleControlChange(byte channel, byte number, byte value);
    void handleClock();
    void check_constraints();
    void handleRotate1(int8_t rotation);
    void handlePress1();
};

#endif
