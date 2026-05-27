/**
 * @file Delay.h
 * @brief Fonctions de l'écho.
 */

#ifndef DELAY_H
#define DELAY_H
#include "../base/Module.h"

#define BUF_SIZE 32 /**< nombre de notes max dans la file FIFO */

class Delay : public Module {
  private:
    typedef struct note {
        byte pitch;
        byte velocity;
        unsigned long p_time;
        boolean noteOn;
        byte feedback;
    } note;

    typedef struct buffer {
        note notes[BUF_SIZE];
        byte head;
        byte queue;
    } buffer;
    enum param { IN, OUT, FBK, DET, TIME, FINE, END_ };
    buffer buf;
    const char *SYNCSTR[10] = {"1/8", "1/6", "1/4", "1/3", "1/2", 
        "2/3", "3/4", "1/1", "4/3", "3/2"};
    double sync[10] = {7500, 10000, 15000, 20000, 30000, 
        40000, 45000, 60000, 80000, 90000};

  public:
    /**
     * @brief Constructeur par défaut.
     */
    Delay(char *name) : Module(name) {
        strcpy(parameters[IN].name, "IN");
        parameters[IN].max = 16;
        strcpy(parameters[OUT].name, "OUT");
        parameters[OUT].max = 16;
        strcpy(parameters[FBK].name, "REPEAT");
        parameters[FBK].value = parameters[FBK].buffer = 1;
        parameters[FBK].max                            = 3;
        strcpy(parameters[TIME].name, "SYNC");
        parameters[TIME].value = parameters[TIME].buffer = 4;
        parameters[TIME].max                             = 8;
        strcpy(parameters[FINE].name, "FINE");
        parameters[FINE].value = parameters[FINE].buffer = 10;
        parameters[FINE].min                             = 0;
        parameters[FINE].max                             = 20; 
        strcpy(parameters[DET].name, "PITCH");
        parameters[DET].value = parameters[DET].buffer = 12;
        parameters[DET].min                            = 0;
        parameters[DET].max                            = 24;
        this->size      = END_; // number of parameters
        this->updatable = true;
        this->setMenu();
        this->buf.head  = 0;
        this->buf.queue = 0;
        this->out       = parameters[OUT].value;
    }

    byte getOut() {
        return parameters[OUT].value;
    }
/**
 * @brief on ajoute la note en fin de la file FIFO.
 */
    void add(byte pitch, byte velocity, unsigned long now_time, bool noteOn,
             byte feedback) {
        this->buf.notes[this->buf.head].pitch    = pitch;
        this->buf.notes[this->buf.head].velocity = velocity;
        this->buf.notes[this->buf.head].p_time   = now_time;
        this->buf.notes[this->buf.head].noteOn   = noteOn;
        this->buf.notes[this->buf.head].feedback = feedback;
        this->buf.head = (this->buf.head + 1) % BUF_SIZE;
    }
/**
 * @brief la hauteur de la note change avec le nombre de répétitions.
 */
    byte changePitch(byte pitch) {
        int val = pitch + this->parameters[DET].value - 12;
        if (0 < val && val <= 96) {
            return (byte)val;
        }
        return pitch;
    }
/**
 * @brief le volume de la note baisse avec le nombre de répétitions.
 */
    byte changeVelocity(byte velocity) {
        if (velocity < 24) {
            return 0;
        }
        return velocity - 24;
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
        case FBK:
            sprintf(str, "%d", parameters[currentIndex].buffer);
            break;
        case TIME:
            sprintf(str, "%s", SYNCSTR[parameters[currentIndex].buffer]);
            break;
        case DET:
            sprintf(str, "%3d", parameters[currentIndex].buffer - 12);
            break;
        case FINE:
            sprintf(str, "%3dms", parameters[currentIndex].buffer - 10);
            break;
        default:
            break;
        }
    }

    double getDelayTime();
    void handleNoteOn(byte channel, byte pitch, byte velocity);
    void handleNoteOff(byte channel, byte pitch, byte velocity);
    void handleClock() {}
    void handle();
    void handlePress1();
};

#endif
