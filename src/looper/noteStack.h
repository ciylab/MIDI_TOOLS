/**
 * @file noteStack.h
 * @brief Fonctions d'une pile de notes.
 */
#ifndef NOTESTACK_H
#define NOTESTACK_H
#include <Arduino.h>
#include "intStack.h"

/**
 * @struct noteStack
 * @brief La structure d'une pile de notes.
 */
typedef struct noteStack {
    byte pitch; /**<hauteur de la note*/
    byte velocity; /**<volume de la note*/ 
    byte level; /**<rang de l'ajout de notes */ 
    bool on; /**<statut de la note on ou off */
    struct noteStack *next;/**<adresse de la note suivante */
} noteStack;

void push(noteStack **, byte, byte, byte, bool);
void pop(noteStack **);
void popLevel(noteStack **, byte);
void undo(byte, intStack *levels[], noteStack *notes[]);
#endif
