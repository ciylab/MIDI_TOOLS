/**
 * @file noteStack.cpp
 * @brief Gestion des piles de notes.
 */

#include "noteStack.h"
#include <Arduino.h> // byte

/**
 * @brief Pour pousser la note sur la pile.
 */
void push(noteStack **s, byte pitch, byte velocity, byte level, bool on) {
    noteStack *element = (noteStack *)malloc(sizeof(noteStack));
    if (element != NULL) { // free RAM size not null !
        element->pitch    = pitch;
        element->velocity = velocity;
        element->level    = level;
        element->on       = on;
        element->next     = *s;
        *s                = element;
    }
}

/**
 * @brief On dépile la note sans la récupérer.
 */
void pop(noteStack **s) {
    noteStack *temp;
    if (*s != NULL) {
        temp = *s;
        *s   = (*s)->next;
        free(temp);
        temp = NULL;
    }
}

/**
 * @brief On dépile toutes les notes d'un niveau.
 */
void popLevel(noteStack **s, byte level) {
    while (*s != NULL && (*s)->level == level) {
        pop(s);
    }
}

/**
 * @brief On supprime toutes les notes d'un niveau.
 *
 * On va chercher les ticks du niveau sur la pile d'entiers.
 */
void deleteNote(intStack **s, byte level, noteStack *notes[]) {
    int i;
    while (*s != NULL) {
        i = popStackOfInt(s);
        popLevel(&notes[i], level);
    }
}

/**
 * @brief On supprime toutes les notes d'un niveau.
 *
 * Cette fois dans le contexte du projet.
 */
void undo(byte level, intStack *levels[], noteStack *notes[]) {
    deleteNote(&levels[level], level, notes);
}
