/*
 * Fichier : build_random_seq.c
 * Date : 2026-06-01
 *
 * Code source pour la génération uniforme de notes aléatoires
 * suivant une tonalité, une gamme et une plage de valeur
 * délimitée par un min et un max. 
 *
 * Par translation on se ramène à la tonalité de C. On compte le nombre
 * a de notes < min et le nombre b de notes < max. On choisit au
 * hasard un nombre entre ces deux valeurs. Ce nombre nous donne le pitchs.
 * 
 * Pour ne pas avoir à tout calculer à chaque fois et générer une 
 * latence MIDI, on stocke le nombre de notes inférieures à une valeur i
 * dans un tableau count.
 * 
 * Les tableaux affichés sont directement importés dans Random.cpp.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NB_SCALE 4

#define CHROMATIC 0
#define MAJOR 1
#define PENTA 2
#define HARM 3

/*
 * Le nombre de notes inférieures ou égales à.
 */
int count[NB_SCALE][12];

/*
 * Booléen absence/présence.
 */
int scales[NB_SCALE][12];

/*
 * Le rang des notes dans la gamme.
 */
int pitchs[NB_SCALE][12] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, // chromatic
  // C  D  E  F  G  A  B
    {0, 2, 4, 5, 7, 9, 11},                 // major
  // C  D  E  G  A
    {0, 2, 4, 7, 9},                        // pentatonic
    {0, 2, 3, 5, 7, 8, 11},                 // harmonic
};

/*
 * Le nombre de notes dans chaque gamme.
 */
int size[NB_SCALE] = {12, 7, 5, 7};

void build_arrays() {
    for(int scale = 0; scale < NB_SCALE; scale++) {
        for(int pitch = 0; pitch < 12; pitch++) {
            scales[scale][pitch] = 0;
        }
        for(int i = 0; i < size[scale]; i++) {
            scales[scale][pitchs[scale][i]] = 1;
        }
        count[scale][0] = 1;
        for(int i = 1; i < 12; i++) {
            count[scale][i] = count[scale][i - 1] + scales[scale][i];
        }
    }
}

void print_arrays() {
    printf("int count[NB_SCALE][12] = {\n");
    for(int scale = 0; scale < NB_SCALE; scale++) {
        printf("    {");
        for(int i = 0; i < 12; i++) {
            printf("%2d, ", count[scale][i]);
        }
        printf("},\n");
    }
    printf("};\n");
    printf("int scales[NB_SCALE][12] = {\n");
    for(int scale = 0; scale < NB_SCALE; scale++) {
        printf("    {");
        for(int i = 0; i < 12; i++) {
            printf("%2d, ", scales[scale][i]);
        }
        printf("},\n");
    }
    printf("};\n");
}

/*
 * Le nombre de notes STRICTEMENT inférieures à une note fixée.
 */
int get_count(int scale, int note) {
    return count[scale][11] * (note / 12) + 
        count[scale][note % 12] -
        scales[scale][note % 12];
}

/*
 * Test : les notes de la gamme majeure strict. inférieures à G1 (19) sont :
 * C0,... C1, D1, E1, F1
 * au nombre de 11.
 */

void test_get_count() {
    assert(get_count(MAJOR, 0) == 0);
    assert(get_count(MAJOR, 1) == 1);
    assert(get_count(MAJOR, 2) == 1);
    assert(get_count(MAJOR, 19) == 11);
}

/*
 * Retourne la hauteur de la note de rang rank dans la gamme.
 */
int get_pitch(int scale, int rank) {
    return 12 * (rank / size[scale]) + pitchs[scale][rank % size[scale]];
}

/*
 * La hauteur de la 3ième note E0 dans la gamme majeure est 5.
 * La hauteur de la 10ième note E1 dans la gamme majeure est 17.
 */
void test_get_pitch() {
    assert(get_pitch(MAJOR, 3) == 5);
    assert(get_pitch(MAJOR, 10) == 17);
}

/*
 * A random number between a include and b exclude
 */
int my_random(int a, int b) {
    return a + rand() % (b - a);
}

/*
 * Retourne une note aléatoire :
 * - scale : la gamme
 * - tone : la clé
 * - min : la borne inférieure
 * - max : la borne supérieure
 */
int get_rand_note(int scale, int tone, int min, int max) {
    int a = get_count(scale, min - tone + 12);
    int b = get_count(scale, max - tone + 12);
    if(a == b) {
        return 0;
    }
    int i = my_random(a, b);
    int note = tone + get_pitch(scale, i);
    if(max <= note) {
        note -= 12;
    }
    return note;
}

/*
 * Les notes en G majeur comprises entre C2 et C3 (exclus) sont :
 * G2, A2, B2, C2, D2, E2, F#2 : 24, 26, 28, 30, 31, 33, 35
 */
void test_get_rand_note() {
    int t[127];
    for(int i = 0; i < 127; i++) {
        t[i] = 0;
    }
    t[24] = t[26] = t[28] = t[30] = t[31] = t[33] =  t[35] = 1;
    for(int i = 0; i < 1000; i++) {
        int note = get_rand_note(MAJOR, 7, 24, 36);
        assert(t[note]);
    }
}

int main() {
    build_arrays();
    print_arrays();
    test_get_count();
    test_get_pitch();
    test_get_rand_note();
    return 0;
}
