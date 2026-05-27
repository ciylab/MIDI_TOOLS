/**
 * @file Sequencer.h
 * @brief Fonctions du petit séquenceur.
 */
#ifndef SEQUENCER_H
#define SEQUENCER_H
#include "../base/Module.h"

#define SEQ_SIZE 5 /**< Taille du séquenceur. */
#define GATE 3 /**< Durée d'une demie double-croche. */

class Sequencer : public Module {
  private:
    const char *NOTES[12] = {"C",  "C#", "D",  "D#", "E",  "F",
                             "F#", "G",  "G#", "A",  "A#", "B"};
    enum param {LEN, OUT, NOTE1, NOTE2, NOTE3, NOTE4, NOTE5, SPEED, END_ };
    bool notes[SEQ_SIZE];
    byte index;

  public:
    /**
     * @brief Constructeur par défaut.
     * */
    Sequencer(char *name) : Module(name) {
        strcpy(parameters[LEN].name, "LENGTH");
        parameters[LEN].max = 5;
        strcpy(parameters[OUT].name, "OUT");
        parameters[OUT].max = 16;
        strcpy(parameters[NOTE1].name, "NOTE 1");
        strcpy(parameters[NOTE2].name, "NOTE 2");
        strcpy(parameters[NOTE3].name, "NOTE 3");
        strcpy(parameters[NOTE4].name, "NOTE 4");
        strcpy(parameters[NOTE5].name, "NOTE 5");
        for (int i = 2; i < 7; i++) {
            parameters[i].value = parameters[i].buffer = MIDDLE_C; 
            parameters[i].min = NOTE_MIN;
            parameters[i].max = NOTE_MAX;
        }
        strcpy(parameters[SPEED].name, "SPEED");
        parameters[SPEED].value = parameters[SPEED].buffer = 1;
        parameters[SPEED].min                              = 1;
        parameters[SPEED].max                              = 16;
        this->size      = END_; // number of parameters
        this->updatable = true;
        this->setMenu();
        this->out = parameters[OUT].value;
        for (int i = 0; i < SEQ_SIZE; i++) {
            this->notes[i] = true;
        }
    }

    byte getOut() {
        return parameters[OUT].value;
    }

    void getString(char str[8]) {
        switch (currentIndex) {
        case LEN:
            sprintf(str, "%2d", parameters[currentIndex].buffer);
            break;
        case OUT:
            if (parameters[currentIndex].buffer == 0) {
                sprintf(str, "%s", "OFF");
            } else {
                sprintf(str, "%2d", parameters[currentIndex].buffer);
            }
            break;
        case SPEED:
            sprintf(str, ":%d", parameters[currentIndex].buffer);
            break;
        default:
            if (!notes[currentIndex - 2]) {
                sprintf(str, "%s", "---");
            } else {
                sprintf(str, "%2s%d",
                        NOTES[parameters[currentIndex].buffer % 12],
                        parameters[currentIndex].buffer / 12 - 1);
            }
            break;
        }
    }
    void handleNoteOn(byte channel, byte pitch, byte velocity) {}
    void handleNoteOff(byte channel, byte pitch, byte velocity) {}
    void handleRotate1(int8_t rotation);
    void handleClock();
    void handlePress1();
};

#endif
