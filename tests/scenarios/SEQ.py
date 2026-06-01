"""
Scénario : on défini une séquence de deux notes à vitesse deux fois
moins rapide.
"""

from utils import *

def init():
    go_to_page(TIME)
    set_parameter_value(1, 127) # clock EXT
    go_to_page(SEQ)
    set_parameter_value(1, 70) # length 2
    set_parameter_value(2, 8) # CHANNEL OUT 1
    set_parameter_value(3, 58) # C4
    set_parameter_value(4, 75) # C5
    set_parameter_value(8, 10) # speed : 2

def test(clock):
    F8_count = 24
    print("************ test clock", clock)
    if clock == "interne":
        go_to_page(TIME)
        set_parameter_value(1, 0) # clock INT
        set_parameter_value(3, 0) # BPM 30
        time.sleep(F8_count * delay) # durée d'une séquence complète
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

