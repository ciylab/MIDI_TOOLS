/**
 * @file Time.h
 * @brief Fonctions du temps.
 */
#ifndef TIME_H
#define TIME_H
#include "../base/Module.h"

#define LED_CLOCK 2 /**<pin de la led */

class Time : public Module {
  private:
    const char *CLOCK[2]  = {"INT", "EXT"};
    const char *ONOFF[2]  = {"OFF", " ON"};
    const char *NOTES[12] = {" C", "C#", " D", "D#", " E", "F",
                             "F#", " G", "G#", " A", "A#", " B"};
    enum param { CLCK, OUT, BPM, METER, 
        FIRST, OTHER, METRONOME, VELOCITY, END_ };

  public:
    /**
     * @brief Constructeur par défauti.
     *
     * Le bpm est compris entre 30 et 240 (Arturia keystep)
     * */
    Time(char *name) : Module(name) {
        strcpy(this->parameters[CLCK].name, "CLOCK");
        parameters[CLCK].value = parameters[CLCK].buffer = 0;
        parameters[CLCK].max                             = 1;
        strcpy(this->parameters[OUT].name, "OUT");
        parameters[OUT].max = 16;
        strcpy(this->parameters[BPM].name, "BPM");
        parameters[BPM].value = parameters[BPM].buffer = BPM_DEFAULT;
        parameters[BPM].min = BPM_MIN;
        parameters[BPM].max = BPM_MAX;
        strcpy(parameters[METER].name, "BAR");
        parameters[METER].max = 6;
        strcpy(this->parameters[FIRST].name, "FIRST");
        parameters[FIRST].value = parameters[FIRST].buffer = 48;
        strcpy(this->parameters[OTHER].name, "BEAT");
        parameters[OTHER].value = parameters[OTHER].buffer = 36;
        parameters[FIRST].min = parameters[OTHER].min = NOTE_MIN;
        parameters[FIRST].max = parameters[OTHER].max = NOTE_MAX;
        strcpy(this->parameters[METRONOME].name, "METRON");
        parameters[METRONOME].max = 1;
        strcpy(this->parameters[VELOCITY].name, "VELCTY");
        parameters[VELOCITY].min = 0;
        parameters[VELOCITY].max = 127;
        parameters[VELOCITY].buffer = DEFAULT_VELOCITY;
        this->size                = END_; // number of parameters
        this->updatable           = true;
        this->setMenu();
        this->out = parameters[OUT].value;
    }

    byte getOut() {
        return parameters[OUT].value;
    }

    void getString(char str[8]) {
        switch (currentIndex) {
        case CLCK:
            sprintf(str, "%s", CLOCK[parameters[currentIndex].buffer]);
            break;
        case OUT:
            if (parameters[currentIndex].buffer == 0) {
                sprintf(str, "%s", "OFF");
            } else {
                sprintf(str, "%2d", parameters[currentIndex].buffer);
            }
            break;
        case BPM:
            sprintf(str, "%2d", parameters[currentIndex].buffer);
            break;
        case METER:
            sprintf(str, "%1d/4", parameters[currentIndex].buffer);
            break;
        case FIRST:
            sprintf(str, "%2s%d", 
                    NOTES[parameters[currentIndex].buffer % 12],
                    parameters[currentIndex].buffer / 12 - 1);
            break;
        case OTHER:
            sprintf(str, "%2s%d", 
                    NOTES[parameters[currentIndex].buffer % 12],
                    parameters[currentIndex].buffer / 12 - 1);
            break;
        case METRONOME:
            sprintf(str, "%s", ONOFF[parameters[currentIndex].buffer]);
            break;
        case VELOCITY:
            sprintf(str, "%2d", parameters[currentIndex].buffer);
            break;
        default:
            break;
        }
    }

    void handleNoteOn(byte channel, byte pitch, byte velocity) {}
    void handleNoteOff(byte channel, byte pitch, byte velocity) {}
    void handleControlChange(byte channel, byte number, byte value);
    void update(int);
    void handleClock();
    void handleRotate1(int8_t rotation);
    void handlePress1();
};

#endif
