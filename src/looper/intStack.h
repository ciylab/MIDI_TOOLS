/**
 * @file intStack.h
 * @brief Fonctions d'une pile d'entiers.
 */
#ifndef STACKOFINT_H
#define STACKOFINT_H

/**
 * @struct intStack
 * @brief La structure d'une pile d'entiers.
 */
typedef struct intStack {
    int value;
    struct intStack *next;
} intStack;

void pushStackOfInt(intStack **, int);
int popStackOfInt(intStack **s);
void printStackOfInt(intStack *s);
#endif
