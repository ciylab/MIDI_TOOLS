import rtmidi
import rtmidi.midiutil
import sys
import time

INPUT_PORT = 1
OUTPUT_PORT = 1
MILLI = 0.5

def io_test():
    rtmidi.midiutil.list_input_ports()
    rtmidi.midiutil.list_output_ports()

def go_to_page(num):
    if 7 < num:
        return False
    midi_out.send_message([0xC0, num])
    time.sleep(0.5)
    return True

def go_to_parameter(num):
    if 8 < num:
        return False
    midi_out.send_message([0xB0, num, 0])
    time.sleep(0.5)
    return True

def change_parameter(num, type):
    if 8 < num:
        return False
    if type == "min":
        midi_out.send_message([0xB0, num, 0])
    elif type == "max":
        midi_out.send_message([0xB0, num, 127])
    time.sleep(0.5)
    return True

def test_navigation():
    for i in range(10):
        if go_to_page(i):
            for j in range(10):
                if go_to_parameter(j):
                    change_parameter(j, "min")
                    change_parameter(j, "max")

if __name__ == '__main__': 
    midi_out = rtmidi.MidiOut()
    midi_out.open_port(OUTPUT_PORT)
    test_navigation()
    midi_out.close_port()
    del midi_out
