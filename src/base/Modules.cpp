/**
 * @file Modules.cpp
 * @brief Liste des modules.
 */

#include "Modules.h"
extern int currentModule;

/**
  * @brief Initialisation de la classe.
  */

Modules::Modules() {
    this->size = 0;
    currentModule = 0;
    menu[0] = CURSOR;
    for (int i = 1; i < 63; i++) {
        menu[i] = ' ';
    }
    menu[63] = '\0';
}

/**
  * @brief Ajoute un module à la liste.
  *
  * Mise à jour conjointe du menu.
  */

void Modules::add(Module *m) {
    if (this->size < 8) {
        modules[this->size] = m;
        int i               = 0;
        while (m->name[i] != '\0' && i < 6) {
            menu[8 * this->size + i + 1] = m->name[i];
            i++;
        }
        this->size++;
    }
}
