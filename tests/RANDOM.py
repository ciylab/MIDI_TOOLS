"""
Scénario : on génère une mélodie aléatoire de 2 notes dans une pentatonique
en D comprise entre C4 et G4. On gèle la séquence pour l'horloge externe.
Pour garantir la reproductabilité, on met la graîne du générateur à 0 dans 
le code source pour les tests. Il faut donc impérativement rebooter le module
pour valider le test.
"""

from utils import *

def init():
    go_to_page(TIME)
    set_parameter_value(1, 127) # clock EXT
    go_to_page(RANDOM)
    set_parameter_value(1, 4) # length = 2
    set_parameter_value(2, 8) # CHANNEL OUT 1
    set_parameter_value(5, 58) # min = C4
    set_parameter_value(6, 68) # max = G4
    set_parameter_value(7, 25) # key D
    set_parameter_value(8, 86) # penta
    return

def test(clock):
    print("************ test clock", clock)
    F8_count = 12
    if clock == "interne":
        go_to_page(TIME)
        set_parameter_value(1, 0) # clock INT
        set_parameter_value(3, 0) # BPM 30
        go_to_page(RANDOM)
        restart()
        time.sleep(F8_count * delay)
        set_parameter_value(4, 127) # freeze
    elif clock == "externe":
        go_to_page(TIME)
        set_parameter_value(1, 127) # clock EXT
        time.sleep(1)
        # on lance une séquence complète
        midi_out.send_message([0xFA]) # début
        time.sleep(delay)
        send_pulse(delay, F8_count)
        midi_out.send_message([0xFC]) # fin
        time.sleep(delay)

if __name__ == '__main__':
    init()
    test("interne")
    test("externe")

