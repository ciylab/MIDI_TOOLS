from scenarios.utils import *

debug = True # affichage des tableaux activé

"""
Le dictionnaire :
    - clé = nom de l'algorithme
    - valeur = sortie MIDI attendue
"""

d = {"TIME": [144, 72, 100] + [248] * 3 + [128, 72, 0] + [248] * 21 + \
        [144, 60, 100] + [248] * 3 + [128, 60, 0] + [248] * 21 + \
        [144, 60, 100] + [248] * 3 + [128, 60, 0] + [248] * 21,
     "SEQ": [144, 60, 100] + [248] * 3 + [128, 60, 0] + [248] * 9 + \
        [144, 72, 100] + [248] * 3 + [128, 72, 0] + [248] * 9,
     "ARP": [144, 60, 100] + [248] * 3 + [128, 60, 0] + [248] * 3 + \
        [144, 72, 100] + [248] * 3 + [128, 72, 0] + [248] * 3,
     "COMPARE": [248] * 6 + [144, 68, 100] + [248] * 3 + \
        [128, 68, 0] + [248] * 3,
     "DELAY": [144, 60, 100] + [248] * 3 + [128, 60, 0] + [248] * 9 + \
        [144, 62, 76] + [248] * 3 + [128, 62, 0] + [248] * 9 + \
        [144, 64, 52] + [248] * 3 + [128, 64, 0] + [248] * 9,
     "EUCLID": [144, 72, 100] + [248] * 3 + [128, 72, 0] + [248] * 3 + \
        [144, 60, 100] + [248] * 3 + [128, 60, 0] + [248] * 9,
     "RANDOM": [144, 64, 100] + [248] * 3 + [128, 64, 0] + [248] * 3 + \
        [144, 66, 100] + [248] * 3 + [128, 66, 0] + [248] * 3,
     "LOOPER": [248] * 3 + [144, 60, 100] + [248] * 3 + [128, 60, 0] + \
             [248] * 18 + \
             [248] * 3 + [144, 60, 100] + [248] * 3 + [128, 60, 0] + \
             [248] * 18
}

expected = d[sys.argv[1]]

def get_output():
    """
    Fonction qui retourne la partie de la sortie MIDI qui intéresse le test
    """
    midi_in = rtmidi.MidiIn()
    midi_in.open_port(INPUT_PORT)
    midi_in.ignore_types(sysex=False, timing=False, active_sense=True)
    result = []
    start = False
    while True:
        msg = midi_in.get_message()
        if msg:
            if msg[0] == [0xFA]:
                start = True
            elif msg[0] == [0xFC]:
                result = []
                start = False
            elif start:
                result += msg[0]
                if len(expected) <= len(result):
                    if debug:
                        print("received:")
                        print(result)
                    break    
    return result

def print_test():
    """
    Simple fonction pour savoir si le test passe.
    """
    if get_output() == expected:
        print("OK")
    else:
        print("KO")

if __name__ == '__main__':
    if debug:
        print("expected:")
        print(expected)
    print_test() # KO ou OK avec horloge interne
    print_test() # KO ou OK avec horloge externe

