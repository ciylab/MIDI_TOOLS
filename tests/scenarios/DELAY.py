"""
Scénario : on défini un delay avec une répétition double, synchronisé à la
croche et un ton en plus pour chaque note répétée.
"""

from utils import *

def init():
    go_to_page(TIME)
    set_parameter_value(1, 127) # clock EXT
    go_to_page(DELAY)
    set_parameter_value(1, 8) # CHANNEL OUT 1
    set_parameter_value(2, 8) # CHANNEL OUT 1
    set_parameter_value(3, 100) # repeat = 2
    set_parameter_value(4, 77) # pitch = +2
    set_parameter_value(5, 70) # sync = 1 / 2    

def test(clock):
    print("************ test clock", clock)
    if clock == "interne":
        go_to_page(TIME)
        set_parameter_value(1, 0) # clock INT
        set_parameter_value(3, 0) # BPM 30
        go_to_page(DELAY)
        restart()
        midi_out.send_message([144, 60, 100]) # note on
        time.sleep(3 * delay)
        midi_out.send_message([128, 60, 0]) # note off
        time.sleep(33 * delay)
    elif clock == "externe":
        go_to_page(TIME)
        set_parameter_value(1, 127) # clock EXT
        time.sleep(1)
        # on lance une séquence complète
        midi_out.send_message([0xFA]) # début
        time.sleep(delay_min)
        midi_out.send_message([144, 60, 100]) # note on
        send_pulse(delay, 3)
        midi_out.send_message([128, 60, 0]) # note off
        send_pulse(delay, 33)
        midi_out.send_message([0xFC]) # fin
        time.sleep(delay)

if __name__ == '__main__':
    init()
    test("interne")
    test("externe")

