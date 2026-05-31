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
    go_to_page(COMPARE)
    set_parameter_value(1, 8) # CHANNEL OUT 1
    set_parameter_value(2, 8) # CHANNEL OUT 1
    set_parameter_value(3, 57) # min = C4
    set_parameter_value(4, 75) # max = C5
    set_parameter_value(5, 34) # + un ton
    

def test(clock):
    print("************ test clock", clock)
    if clock == "interne":
        go_to_page(TIME)
        set_parameter_value(1, 0) # clock INT
        set_parameter_value(3, 0) # BPM 30
        go_to_page(COMPARE)
        restart()
        midi_out.send_message([144, 59, 100]) # note on
        time.sleep(3 * delay)
        midi_out.send_message([128, 59, 0]) # note off
        time.sleep(3 * delay)
        midi_out.send_message([144, 66, 100]) # note on
        time.sleep(3 * delay)
        midi_out.send_message([128, 66, 0]) # note off
        time.sleep(3 * delay)
        midi_out.send_message([144, 76, 100]) # note on
        time.sleep(3 * delay)
        midi_out.send_message([128, 76, 0]) # note off
        time.sleep(3 * delay)
    elif clock == "externe":
        go_to_page(TIME)
        set_parameter_value(1, 127) # clock EXT
        time.sleep(1)
        # on lance une séquence complète
        midi_out.send_message([0xFA]) # début
        time.sleep(delay)
        midi_out.send_message([144, 59, 100]) # note on
        send_pulse(delay, 3)
        midi_out.send_message([128, 59, 0]) # note off
        send_pulse(delay, 3)
        midi_out.send_message([144, 66, 100]) # note on
        send_pulse(delay, 3)
        midi_out.send_message([128, 66, 0]) # note off
        send_pulse(delay, 3)
        midi_out.send_message([144, 76, 100]) # note on
        send_pulse(delay, 3)
        midi_out.send_message([128, 76, 0]) # note off
        send_pulse(delay, 3)
        midi_out.send_message([0xFC]) # fin
        time.sleep(delay)

if __name__ == '__main__':
    init()
    test("interne")
    test("externe")





    
    

"""
def test_horloge(internal):
    midi_out = rtmidi.MidiOut()
    midi_out.open_port(OUTPUT_PORT)
    init(midi_out, TIME, [127, 3 * 128 / 16, 0, 4 * 128 / 6, 127, 0, 0])
    time.sleep(2*milli)     
    if (internal):
        midi_out.send_message([0xF0, TIME, 0, 0xF7])
        time.sleep(milli)  
        midi_out.send_message([0xB0, 1, 0])        # clock INT
        midi_out.send_message([0xB0, 3, 0])        # bpm 30
        time.sleep(24 * delay)
        midi_out.send_message([0xB0, 1, 127])      # clock EXT 
    else:
        midi_out.send_message([0xF0, TIME, 1, 0xF7])
        midi_out.send_message([0xFA])
        time.sleep(milli)  
        send_delay(24, delay, midi_out)
        midi_out.send_message([0xFC])
    del midi_out

def test_metronome(internal):
    midi_out = rtmidi.MidiOut()
    midi_out.open_port(OUTPUT_PORT)
    init(midi_out, TIME, [127, 3 * 128 / 16, 0, 4 * 128 / 6, 127, 0, 127])
    if (internal):
        midi_out.send_message([0xF0, TIME, 2, 0xF7])
        midi_out.send_message([0xB0, 1, 0])        # clock INT
        midi_out.send_message([0xB0, 3, 0])        # bpm 30
        time.sleep(96 * delay)
        midi_out.send_message([0xB0, 1, 127])      # clock EXT
    else:
        midi_out.send_message([0xF0, TIME, 3, 0xF7])
        midi_out.send_message([0xFA])
        time.sleep(milli)
        send_delay(96, delay, midi_out)
        midi_out.send_message([0xFC])
    midi_out.send_message([0xB0, 7, 0])        # metronome off
    del midi_out
"""
