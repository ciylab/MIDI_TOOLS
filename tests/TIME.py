"""
Scénario : on teste essentiellement le métronome. Clock interne ou 
externe à 30 bpm le résultat doit être le même.
"""

from utils import *

def init():
    go_to_page(TIME)
    set_parameter_value(1, 127) # clock EXT
    set_parameter_value(2, 8) # CHANNEL OUT 1
    set_parameter_value(4, 64) # 3/4
    set_parameter_value(5, 75) # C5
    set_parameter_value(6, 57) # C4
    set_parameter_value(7, 127) # metro. ON

def test(clock):
    F8_count = 24 * 3
    print("************ test clock", clock)
    if clock == "interne":
        set_parameter_value(1, 0) # clock INT
        set_parameter_value(3, 0) # BPM 30
        time.sleep(F8_count * delay) # durée d'une séquence complète
    elif clock == "externe":
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
