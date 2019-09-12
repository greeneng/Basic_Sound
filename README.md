# Basic_Sound
Demo library for playing monophonic melodies stored in strings with QuickBasic-like format with Arduino

This library plays simple monophonic melodies using Arduino's Tone() function and piezo-buzzer. Unlike many examples which use a bunch of arrays to store individual notes characteristics, this project uses null-terminated strings formatted very similar to QuickBasic/QBasic PLAY operator which has much greater readability. Background playing ("MB" command) requiring using interrupts is not implemented.

See MegaSound.ino Arduino project as example
