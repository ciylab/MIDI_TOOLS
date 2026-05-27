/**
 * @file Display.cpp
 * @brief Gestion de l'affichage.
 *
 * La variable screen représente ce qui est affiché.
 * La variable buffer est ce qui doit être affiché.
 * Pour des questions de latence, screen est mis à jour à partir
 * de buffer tant qu'elles sont différentes et un caractère
 * par boucle de loop(). 
 */


#include "Display.h"
#include <U8x8lib.h>
#include "my_u8x8_font_7x14_1x2_r.h"

/**
 * Oled SSD1306 ou SH1106.
 */

#ifdef SSD1306
U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);
#elif SH1106
U8X8_SH1106_128X64_NONAME_HW_I2C u8x8(U8X8_PIN_NONE);
#endif
extern int currentIndex;

/**
 * @brief Initialisation avec 64 espaces.
 */
Display::Display() {
    for (int i = 0; i < 63; i++) {
        screen[i] = ' ';
    }
    screen[63] = '\0';
    u8x8.setFont(my_u8x8_font_7x14_1x2_r);
}

/**
 * @brief Initialisation du setup.
 */
void Display::begin() {
    u8x8.begin();
}
    
/**
 * @brief Affiche un caractère **c** à une position **position**.
 */
void Display::putChar(byte position, char c) {
    u8x8.drawGlyph(position % 16, 2 * (position / 16), c);
}

/**
 * @brief Affiche un message de bienvenue.
 * */

void Display::welcome() {
    sprintf(screen, "  MIDI TOOLS      FOR SYNTH       BY CIYLAB       %s",
            VERSION);
    for (int i = 0; i < 63; i++) {
        putChar(i, screen[i]);
    }
}

/**
 * @brief Fonction de rafraîchissement.
 *
 * La variable **charIndex** est le numéro de caractère courant à afficher.
 * La variable **endPosition** est le numéro du dernier caractère du
 * buffer à afficher. En général il y a un groupe de 6 caractères seulement
 * à rafraîchir. La fonction en affiche un au maximum.
 * */

void Display::display() {
    // without buffer : 100ms
    // with buffer : 2ms
    if (endPosition <= charIndex) {
        return;
    }
    while (charIndex < endPosition && 
        screen[charIndex] == buffer[charIndex]) {
        charIndex++;
    }
    putChar(charIndex, buffer[charIndex]);
    screen[charIndex] = buffer[charIndex];
    charIndex++;
}

/**
 * @brief Déplace le curseur.
 *
 * On met un espace là où on est puis un chevron là où on va.
 * */

void Display::showSelected(int8_t rotation, int size) {
    putChar(8 * currentIndex, ' ');
    if (rotation > 0) {
        currentIndex = (currentIndex + 1) % size;
    } else if (rotation < 0) {
        currentIndex = (currentIndex - 1 + size) % size;
    } else {
        currentIndex = 0;
    }
    putChar(8 * currentIndex, CURSOR);
}

/**
 * @brief Met le nom du paramètre (item) dans le buffer.
 *
 * Initialise **charIndex** et **endPosition**.
 * */

void Display::showItem(Module *m) {
    for (int i = 1; i < 7; i++) {
        int j = 8 * currentIndex + i;
        buffer[j] = m->menu[j];
    }
    charIndex = 8 * currentIndex + 1;
    endPosition = charIndex + 6;
}

/**
 * @brief Met la value du paramètre dans le buffer.
 *
 * Initialise **charIndex** et **endPosition**.
 * */

void Display::showValue(Module *m) {
    char temp[8];
    int i = 0;
    m->getString(temp);
    while (i < 6 && temp[i] != '\0') {
        buffer[8 * currentIndex + 1 + i] = temp[i];
        i++;
    }
    while (i < 6) {
        buffer[8 * currentIndex + 1 + i] = ' ';
        i++;
    }
    charIndex = 8 * currentIndex + 1;
    endPosition = charIndex + 6;
}
