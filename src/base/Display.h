/**
 * @file Display.h
 * @brief Champs de la classe d'affichage.
 */
 
#ifndef DISPLAY_H
#define DISPLAY_H
#include "Modules.h"

class Display {
  public:
    char buffer[64]; /**<tampon de l'écran */ 
    char screen[64]; /**<contenu de l'écran */
    char menu[64]; /**<menu du module courant */
    byte charIndex; /**<position dans le tampon */
    byte endPosition; /**<dernière position à afficher */
    Display();
    void begin();
    void putChar(byte position, char c);
    void welcome();
    void display();
    void showSelected(int8_t rotation, int size);
    void showItem(Module *m);
    void showValue(Module *m);
};

#endif
