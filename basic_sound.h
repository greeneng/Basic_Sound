/*
 *   basic_sound.h - a simple library for playing melodies stored inside null-terminated string
 *   in format similar to PLAY operator of Basic programming language
 *   with piezo buzzer.
 *  
 *  uses Arduino Tone() function and tutorial https://www.arduino.cc/en/Tutorial/ToneMelody
 *  
 *  sorry for class name TNotePlayer not corresponding with file name
 *  
 *  Created by Andrey Simonov, 2019.
 */

#include "arduino.h"

// comment this to disable debug via Serial
#define BASIC_SOUND_DEBUG

typedef enum { ModeNormal, ModeLegato, ModeStaccato } TPlayerMode;
typedef enum { AlterNormal, AlterFlat, AlterSharp, AlterDefault } TPlayerAlteration;

class TNotePlayer
{
  public:
    TNotePlayer();        // initialize with default pin (2)
    TNotePlayer(int pin); // initialize with user-defined pin

    // play string in QuickBasic format
    // differences:
    //   "X" + VARPTR$(scale$) is not needed.
    //   MF and MB commands are NOT supported, music plays only in foreground
    // additions:
    //   +1 octave (O7, N85 - N90)
    //   M#, M- for overall (global) semitone alteration
    //   suffix ' for natural note when global semitone is setted
    void playString(char* str); 

    // stop playing from the current pin
    void stop();

    // stop playing and set new pin for output
    void setPin(int pin);

  private:
    // hardware settings
    int _pwm_pin;
    // global mode settings, set via commands inside string
    int Tempo;
    int Duration;
    int Octave;
    TPlayerMode Mode;
    TPlayerAlteration Alteration;

    // individual note properties
    int Note;             // note number 1 - 90, 0 for pause
    double NoteDuration;
    double NoteTime;

    //int keyFreq(int key);           // obsolete, use noteToFreq instead
    float noteToFreq(int note);       // get frequency of note based on it's number: note=1 = C1 (32.7032 Hz), Note=84 = B7 (3951.07 Hz)
    int Name2SemitonesFromC(char c);  // calculate number of semitones between base note name and 'C'
    void playNote(int len, TPlayerAlteration alter, int dots); // play individual note 
    void _Init();  
};
