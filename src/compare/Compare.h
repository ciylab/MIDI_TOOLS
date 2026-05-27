#ifndef COMPARE_H
#define COMPARE_H
#include "../base/Module.h"

class Compare : public Module {
  private:
    const char *NOTES[12] = {" C", "C#", " D", "D#", " E", "F",
                             "F#", " G", "G#", " A", "A#", " B"};
    enum param { IN, OUT, MIN, MAX, TRANSP, END_ };

  public:
    /**
     * @brief Constructeur par défaut.
     */
    Compare(char *name) : Module(name) {
        strcpy(parameters[IN].name, "IN");
        parameters[IN].max = 16;
        strcpy(parameters[OUT].name, "OUT");
        parameters[OUT].max = 16;
        strcpy(this->parameters[MIN].name, "MIN");
        parameters[MIN].value = parameters[MIN].buffer = NOTE_MIN;
        parameters[MIN].min                            = NOTE_MIN;
        parameters[MIN].max                            = NOTE_MAX;
        strcpy(this->parameters[MAX].name, "MAX");
        parameters[MAX].value = parameters[MAX].buffer = NOTE_MAX;
        parameters[MAX].min                            = NOTE_MIN;
        parameters[MAX].max                            = NOTE_MAX;
        strcpy(parameters[TRANSP].name, "KEY");
        parameters[TRANSP].max = 11;
        this->size             = END_; // number of parameters
        this->updatable        = true;
        this->out              = parameters[OUT].value;
        this->setMenu();
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
        case TRANSP:
            sprintf(str, "%2s", NOTES[parameters[currentIndex].buffer]);
            break;
        default:
            break;
        }
    }

    void handleNoteOn(byte channel, byte pitch, byte velocity);
    void handleNoteOff(byte channel, byte pitch, byte velocity);
    void handleClock() {}
    void handleRotate1(int8_t rotation);
    void handlePress1();
};

#endif
