/**
 * @file Arp.h
 * @brief Fonctions de l'arpégiateur.
 */

#ifndef ARP_H
#define ARP_H
#include "../base/Module.h"

/**taille de la mémoire pour enregistrer 4 mesures de 16 double-croches */
#define A_SEQ_SIZE 64

class Arp : public Module {
  private:
    typedef struct note {
        byte pitch;
        byte velocity;
    } note;

    typedef struct sequence {
        note notes[A_SEQ_SIZE];
        int size;
        int index; // current position
    } sequence;

    const char *ONOFF[2]   = {"OFF", " ON"};
    const int TRANSPOSE[5] = {-12, -5, 0, +7, +12};
    const char *NOTES[12]  = {" C", "C#", " D", "D#", " E", "F",
                              "F#", " G", "G#", " A", "A#", " B"};
    enum param { IN, OUT, RECORD, PLAY, MOD, P_JUMP, GATE, TRANSP, END_ };
    sequence seq;

  public:
    /**
     * @brief Constructeur par défaut.
     * */
    Arp(char *name) : Module(name) {
        strcpy(parameters[IN].name, "IN");
        parameters[IN].max = 16;
        strcpy(parameters[OUT].name, "OUT");
        parameters[OUT].max = 16;
        strcpy(parameters[PLAY].name, "PLAY");
        parameters[PLAY].max = 1;
        strcpy(parameters[RECORD].name, "RECORD");
        parameters[RECORD].max = 1;
        strcpy(parameters[MOD].name, "CHANGE");
        parameters[MOD].max = 10;
        strcpy(parameters[P_JUMP].name, "JUMP");
        parameters[P_JUMP].max = 10;
        strcpy(parameters[GATE].name, "GATE");
        parameters[GATE].value = parameters[GATE].buffer = 3;
        parameters[GATE].max                             = 5;
        parameters[GATE].min                             = 1;
        strcpy(parameters[TRANSP].name, "KEY");
        parameters[TRANSP].max = 11;
        this->size             = END_; // number of parameters
        this->seq.size         = 0;
        this->updatable        = true;
        this->out              = parameters[OUT].value;
        this->setMenu();
    }

    byte getOut() {
        return parameters[OUT].value;
    }
    
/**
 * @brief Ajoute la note à l'arpégiateur.
 * */

    void add(byte pitch, byte velocity) {
        if (seq.size < A_SEQ_SIZE) {
            seq.notes[seq.size].pitch    = pitch;
            seq.notes[seq.size].velocity = velocity;
            seq.size++;
        }
    }
    
/**
 * @brief Retourne un pitch plus ou moins aléatoire.
 *
 * On reste dans la tonalité (octave, quarte ou quinte).
 * */

    byte changePitch(byte pitch) {
        if (random(10) < 10 - this->parameters[MOD].value) {
            return pitch;
        }
        return pitch + TRANSPOSE[random(5)];
    }
    
/**
 * @brief Si vrai ne joue pas la note.
 * */

    byte changeVelocity(byte velocity) {
        if (random(10) < 10 - this->parameters[MOD].value) {
            return velocity;
        }
        return velocity / 2;
    }
    
/**
 * @brief Si vrai ne joue pas la note.
 * */
    bool jump() {
        return random(10) < this->parameters[P_JUMP].value;
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
        case PLAY:
            sprintf(str, "%s", ONOFF[parameters[currentIndex].buffer]);
            break;
        case RECORD:
            sprintf(str, "%s", ONOFF[parameters[currentIndex].buffer]);
            break;
        case MOD:
            setProgressString(str, parameters[currentIndex].buffer);
            break;
        case P_JUMP:
            setProgressString(str, parameters[currentIndex].buffer);
            break;
            break;
        case GATE:
            setProgressString5(str, parameters[currentIndex].buffer);
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
    void handleControlChange(byte channel, byte number, byte value) 
        override;
    void handleClock();
    void handleRotate1(int8_t rotation);
    void handlePress1();
};

#endif
