# fichier : utils.py
# date : 2026-05-31
#
# Fichier contenant des fonctions de base pour les tests ainsi qu'un test
# permettant visuellement de s'assurer que la navigation par PC et
# CC est correcte.

import rtmidi
import rtmidi.midiutil
import sys
import time

"""
Numéros des pages du module qui servent d'arguments pour les scripts.
"""
TIME = 0
SEQ = 1
ARPEGGIATOR = 2
COMPARE = 3
DELAY = 4
LOOPER = 5
EUCLID = 6
RANDOM = 7

INPUT_PORT = 1
OUTPUT_PORT = 1

bpm = 30 # le plus petit bpm pour aider la lecture du MIDI OUT
delay = 60 / bpm / 24 # delay entre deux signaux d'horloge

# 320 ms minimum entre les octets en MIDI et est aussi utile pour 
# la transmission des données OLED
delay_min = 0.01

midi_out = rtmidi.MidiOut()
midi_out.open_port(OUTPUT_PORT)

def io_test():
    """
    Pour avoir la liste des ports disponible.
    """
    rtmidi.midiutil.list_input_ports()
    rtmidi.midiutil.list_output_ports()

def go_to_page(num):
    """
    Fonction pour changer de page pendant les tests.
    """
    if 7 < num:
        return False
    midi_out.send_message([0xC0, num])
    time.sleep(delay_min)
    return True

def go_to_parameter(num):
    """
    Fonction pour activer un paramètre. On appelle qu'une précaution
    a été prise pour ne pas modifier le pramaètre à la première rotation 
    ou pression.
    """
    if 8 < num:
        return False
    midi_out.send_message([0xB0, num, 0])
    time.sleep(delay_min)
    return True

def set_parameter_value(num, value):
    """
    Modifie la valeur du paramètre par CC de 1 à 8 maximum.
    """
    if go_to_parameter(num):
        midi_out.send_message([0xB0, num, value])
        time.sleep(delay_min)
        return True
    return False

def ending():
    """
    Pour quitter "proprement" ?
    """
    midi_out.close_port()
    del midi_out

def midi_print(message):
    """
    Fonction d'affichage au format amidi (hexadécimal).
    """
    for val in message:
        if val == 0xF8:
            print("\n" + str(val), end = ' ')
        else:
            print(val, end=' ')

def send_pulse(delay, F8_count):
    """
    Utilisé pour simuler la clock externe.
    """
    for i in range(F8_count):
        midi_out.send_message([0xF8])
        time.sleep(delay)

def restart():
    """
    Flag pour indiquer le démarrage de la lecture des signaux midi
    reçus dans les tests avec horloge interne.
    """
    midi_out.send_message([0xFC])
    time.sleep(delay_min)
    midi_out.send_message([0xFA])
    time.sleep(delay_min)

def test_navigation():
    """
    Fonction pour un test visuel de la navigation.
    """
    global delay_min
    # redéfinition car la valeur par défaut ne permet pas la lecture
    delay_min = 0.1     
    for i in range(10):
        if go_to_page(i):
            for j in range(10):
                if go_to_parameter(j):
                    set_parameter_value(j, 0)
                    set_parameter_value(j, 127)

if __name__ == '__main__': 
    test_navigation()
