"""
Scénario : on défini un range avec min et max. On joue une note augmentée 
d'un ton :
- < min
- comprise entre min et max
- > max
"""

from utils import *

def init():
    go_to_page(TIME)
    set_parameter_value(1, 127) # clock EXT
    go_to_page(EUCLID)
    set_parameter_value(1, 30) # length = 4
    set_parameter_value(2, 8) # CHANNEL OUT 1
    set_parameter_value(3, 75) # min = C5
    set_parameter_value(4, 57) # max = C4
    set_parameter_value(5, 44) # 1 beat
    set_parameter_value(6, 61) # 1 beat
    set_parameter_value(8, 40) # shift = 1

def test(clock):
    print("************ test clock", clock)
    F8_count = 24
    if clock == "interne":
        go_to_page(TIME)
        set_parameter_value(1, 0) # clock INT
        set_parameter_value(3, 0) # BPM 30
        go_to_page(EUCLID)
        restart()
        time.sleep(F8_count * delay)
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

