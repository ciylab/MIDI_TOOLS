/**
 * @file intStack.cpp
 * @brief Gestion de la pile d'entiers.
 *
 * La pile contient le numéro des ticks d'un level donné.
 */
#include "intStack.h"
#include <Arduino.h> // malloc

/**
 * @brief Pour pousser l'entier sur la pile.
 */
void pushStackOfInt(intStack **s, int value) {
    intStack *element = (intStack *)malloc(sizeof(intStack));
    if (element != NULL) { // free RAM size not null !
        element->value = value;
        element->next  = *s;
        *s             = element;
    }
}

/**
 * @brief On dépile l'entier avec possibilité de le récupérer.
 */
int popStackOfInt(intStack **s) {
    intStack *temp;
    int value;
    if (*s != NULL) {
        value = (*s)->value;
        temp  = *s;
        *s    = (*s)->next;
        free(temp);
        temp = NULL;
    }
    return value;
}

/**
 * @brief Affichage pour tester.
 */
void printStackOfInt(intStack *s) {
    while (s != NULL) {
        Serial.print(s->value);
        Serial.print("->");
        s = s->next;
    }
    Serial.print("\n");
}
