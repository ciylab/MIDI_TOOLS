#ifndef MIDI_H
#define MIDI_H

void handleNoteOn(byte channel, byte pitch, byte velocity);
void handleNoteOff(byte channel, byte pitch, byte velocity);
void handleControlChange(byte channel, byte number, byte value);
void handleProgramChange(byte channel, byte number);
void handleSystemExclusive(byte *, unsigned int);
void handleClock();
void handleStart();
void handleStop();
void handleTime();

#endif
