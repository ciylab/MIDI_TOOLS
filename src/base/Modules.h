/**
 * @file Modules.h
 * @brief Gestion de la liste des modules.
 */

#ifndef MODULES_H
#define MODULES_H
#include "Module.h"

class Modules {
  public:
    int size;
    char menu[64]; /**<menu principal */
    Module *modules[8]; /**<liste des modules */
    Modules();
    void add(Module *);
    void update();
};

#endif
