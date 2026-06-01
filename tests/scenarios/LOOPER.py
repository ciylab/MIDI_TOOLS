"""
Scénario : on enregistre une note en mode clock interne sur un temps puis 
on lance la séquence. Séquence toujours active, on passe en mode 
clock externe.
"""

from utils import *

def init():
    go_to_page(TIME)
    set_parameter_value(1, 127) # clock EXT
    go_to_page(LOOPER)
    set_parameter_value(1, 8) # CHANNEL OUT 1
    set_parameter_value(2, 8) # CHANNEL OUT 1
    set_parameter_value(6, 0) # length = 4

def test(clock):
    F8_count = 48
    print("************ test clock", clock)
    if clock == "interne":
        go_to_page(TIME)
        set_parameter_value(1, 0) # clock INT
        set_parameter_value(3, 0) # BPM 30
        go_to_page(LOOPER)
        restart()
        set_parameter_value(3, 127) # recording on
        time.sleep(3 * delay)
        midi_out.send_message([144, 60, 100]) # note on
        time.sleep(3 * delay)
        midi_out.send_message([128, 60, 0]) # note off
        set_parameter_value(3, 0) # recording off
        time.sleep((F8_count - 6) * delay) # durée d'une séquence complète
    elif clock == "externe":
        go_to_page(TIME)
        set_parameter_value(1, 127) # clock EXT
        # on lance une séquence complète
        midi_out.send_message([0xFA]) # début
        send_pulse(delay, F8_count)
        midi_out.send_message([0xFC]) # fin
        time.sleep(delay)

if __name__ == '__main__':
    init()
    test("interne")
    test("externe")

