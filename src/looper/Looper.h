/**
 * @file Looper.h
 * @brief Fonctions du séquenceur.
 */

#ifndef LOOPER_H
#define LOOPER_H
#include "../base/Module.h"
#include "noteStack.h"

#define LOOPER_SIZE 384 /**<4 mesures de PPQN max. */
#define LEVEL_SIZE 8 /**<nombre de UNDO possibles. */

class Looper : public Module {
  private:
    const int LENGTH[9]  = {4, 6, 8, 12, 16, 24, 32, 48, 64};
    const char *ONOFF[2] = {"OFF", " ON"};
    int top_level; // level max in stack
    intStack *levels[LEVEL_SIZE];
    noteStack *notes[LOOPER_SIZE];
    boolean empty_recording;
    bool playing[128];
    enum param { IN, OUT, RECORD, UNDO, DEL, LEN, END_ };

  public:
    /**
     * @brief Constructeur par défaut.
     * */
    Looper(char *name) : Module(name) {
        this->size = 0;
        strcpy(parameters[IN].name, "IN");
        parameters[IN].max = 16;
        strcpy(parameters[OUT].name, "OUT");
        parameters[OUT].max = 16;
        strcpy(parameters[RECORD].name, "RECORD");
        parameters[RECORD].max = 1;
        strcpy(parameters[UNDO].name, "UNDO");
        parameters[UNDO].max = 1;
        strcpy(parameters[DEL].name, "DELETE");
        parameters[DEL].max = 1;
        strcpy(parameters[LEN].name, "LENGTH");
        parameters[LEN].value = parameters[LEN].buffer = 4;
        parameters[LEN].max                            = 8;
        this->size      = END_; // number of parameters
        this->updatable = true;
        this->setMenu();
        this->tick            = 0;
        this->top_level       = 0;
        this->empty_recording = false;
        for (int i = 0; i < LOOPER_SIZE; i++) {
            notes[i] = NULL;
        }
        for (int i = 0; i < LEVEL_SIZE; i++) {
            levels[i] = NULL;
        }
        for (int i = 0; i < 128; i++) {
            playing[i] = false;
        }
        this->out = parameters[OUT].value;
    }

    byte getOut() {
        return parameters[OUT].value;
    }

    void getString(char str[8]) {
        switch (currentIndex) {
        case IN:
            if (parameters[currentIndex].buffer == 0) {
                sprintf(str, "%s", "OFF");
            } else {
                sprintf(str, "%2d", parameters[currentIndex].buffer);
            }
            break;
        case OUT:
            if (parameters[currentIndex].buffer == 0) {
                sprintf(str, "%s", "OFF");
            } else {
                sprintf(str, "%2d", parameters[currentIndex].buffer);
            }
            break;
        case UNDO:
            sprintf(str, "%s", ONOFF[parameters[currentIndex].buffer]);
            break;
        case RECORD:
            sprintf(str, "%s", ONOFF[parameters[currentIndex].buffer]);
            break;
        case DEL:
            sprintf(str, "%s", ONOFF[parameters[currentIndex].buffer]);
            break;
        case LEN:
            sprintf(str, "%d", LENGTH[parameters[currentIndex].buffer]);
            break;
        default:
            break;
        }
    }

    void handleNoteOn(byte channel, byte pitch, byte velocity);
    void handleNoteOff(byte channel, byte pitch, byte velocity);
    void handleControlChange(byte channel, byte number, byte value) 
        override;
    void handleClock();
    void handleRotate1(int8_t rotation);
    void handlePress1();
    void clean();
    void play();
};

#endif
