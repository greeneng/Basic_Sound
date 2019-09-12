#include "basic_sound.h"
#include "notes.h"

// default values for constructor
#define SPEAKER 2
#define TEMPO 100
#define BASE_LEN ((60000 / TEMPO) * 4)
#define STACCATO (3/4)
#define LEGATO 1
#define NORMAL (7/8)
#define ALTER_FLAT 0.9438
#define ALTER_SHARP 1.0595
#define MAX_STR_LEN 4096

// hardware declarations
#define NOP __asm__ __volatile__ ("nop\n\t")
// 168 and 328 Arduinos
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) 
  #define MAX_PIN 53
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  #define MAX_PIN 13 
#endif

/*
int TNotePlayer::keyFreq(int key)
{
  float freq = 440;
  if (key < 1)
    return 0;
  else if ((key > 47) && (key < 90))
  {
    for (int i = 0; i < (key - 47); i++)
    {
      freq *= ALTER_SHARP;
    }
  }
  else
  {
    for (int i = 0; i < (47 - key); i++)
    {
      freq *= ALTER_SHARP;
    }
  }
  return freq;
}
*/

float TNotePlayer::noteToFreq(int note)
{
  float f = 0;
  if ((note > 0) && (note <= 91))
    f = 440 * exp(log(2) * (note - 46) / 12.0);
  return f;
}

int TNotePlayer::Name2SemitonesFromC(char c)
{
  static const int semitonesFromC[7] = { 9, 11, 0, 2, 4, 5, 7 }; // A,B,C,D,E,F,G
  if (c < 'A' && c > 'G') return -1;
  return semitonesFromC[c - 'A'];
}

void TNotePlayer::playNote(int len, TPlayerAlteration alter, int dots)
{
  int semitone = this->Note;
  float duration = 60000.0 / float(this->Tempo) * 4.0;
  if (len == 0)
    duration = duration / this->Duration;
  else
    duration = duration / len;
  for (int i = 0; i < dots; i++)
    duration = duration * 1.5;
  if (alter != AlterNormal)
  {
     if (this->Alteration == AlterFlat)
       semitone--;
     else if (this->Alteration == AlterSharp)
       semitone++;
     if (alter == AlterFlat)
       semitone--;
     else if (alter == AlterSharp)
       semitone++;
  }   
  float freq = noteToFreq(semitone);
  
  unsigned long ms = millis();
  
  if (this->Mode == ModeNormal)
    tone(_pwm_pin, round(freq), round(duration * 7 / 8.0));
  else if (this->Mode == ModeLegato)
    tone(_pwm_pin, round(freq), round(duration));  
  else if (this->Mode == ModeStaccato)
    tone(_pwm_pin, round(freq), round(duration * 3 / 4.0));
  
  #if defined(BASIC_SOUND_DEBUG)
    Serial.print(semitone);
    Serial.print(" ");
    Serial.print(len);
    Serial.print(" ");
    Serial.print(freq);
    Serial.print(" ");
    Serial.println(dots);
  #endif
  while (millis() - ms < duration)
  {
    NOP;
  }  
}


void TNotePlayer::playString(char* str)
{
  char* ptr = str;
  char ch = toupper(*ptr);
  int count = 0;
  while ((ch != '\0') && (count < MAX_STR_LEN))
  {
    //Serial.print(ch);
    count++;
    //Serial.print(ch);
    if (ch == 'O')
    {
      ptr++;
      ch = *ptr;
      if ((ch >= '0') && (ch <= '7'))
        this->Octave = ch - '0';
      else
        break;      
      //Serial.println(this->Octave);  
    }
    else if (ch == '<')
    {
      if (this->Octave > 0) this->Octave--;
      //Serial.println(ch);
    }
    else if (ch == '>')
    {
      if (this->Octave < 7) this->Octave++;
      //Serial.println(ch);
    }
    else if (ch == 'M')
    {
      ptr++;
      ch = toupper(*ptr);
      if (ch == 'L')
        this->Mode = ModeLegato;
      else if (ch == 'N')
        this->Mode = ModeNormal;
      else if (ch == 'S')
        this->Mode = ModeStaccato;
      else if ((ch == '#') || (ch == '+'))
        this->Alteration = AlterSharp;
      else if (ch == '-')
        this->Alteration = AlterFlat;
      else if (ch == '\'')
        this->Alteration = AlterNormal;
      else
        break;  
      //Serial.println(ch);
    }
    else if ((ch == 'L') || (ch == 'T') || (ch == 'N'))
    {      
      char cmd = ch;
      int number =  0;
      do
      {
        ptr++;
        ch = toupper(*ptr);
        if (isdigit(ch))
          number = number * 10 + (ch - '0');
      }
      while (isdigit(ch));
     
      if ((cmd == 'L') && (number >= 1) && (number <= 64))
        this->Duration = number;
      else if ((cmd == 'T') && (number >= 32) && (number <= 255))
        this->Tempo = number;
      else if ((cmd == 'N') && (number >= 0) && (number <= 99))
      {
        this->Note = number;
        playNote(0, AlterDefault, 0);  
      }
      //Serial.println(number);
      continue;    
    }
    else if (((ch >= 'A') && (ch <= 'G')) || (ch == 'P'))
    {
      if (ch == 'P')
        this->Note = 0;
      else
        this->Note = 1 + this->Octave * 12 + Name2SemitonesFromC(ch);
         
      ptr++;
      ch = toupper(*ptr);      
      TPlayerAlteration alt = AlterDefault;
      if ((ch == '+') || (ch == '#'))
      {
        alt = AlterSharp;
        ptr++;
        ch = toupper(*ptr);
      }
      else if (ch == '-')
      {
        alt = AlterFlat;
        ptr++;
        ch = toupper(*ptr);
      }
      else if (ch == '\'')
      {
        alt = AlterNormal;
        ptr++;
        ch = toupper(*ptr);
      }  
      
      int number =  0;
      while (isdigit(ch))
      {
        number = number * 10 + (ch - '0');
        ptr++;
        ch = toupper(*ptr);
      } 
      int dots = 0; 
      while (ch == '.')
      {
        dots++;
        ptr++;
        ch = toupper(*ptr);
      }      
      playNote(number, alt, dots);
      continue;
    }
    ptr++;
    ch = toupper(*ptr);
  }
}

void TNotePlayer::stop()
{
  noTone(_pwm_pin);
}

void TNotePlayer::setPin(int pin)
{
  stop();
  if (pin < MAX_PIN) 
    this->_pwm_pin = pin;
  pinMode(this->_pwm_pin, OUTPUT);
}

void TNotePlayer::_Init()
{
  this->Tempo = 120; // [32,255] quarter notes per minute
  this->Duration = 4; // [1,64]
  this->Octave = 4; // [0,6]
  this->Mode = ModeNormal;
  this->Note = 0;
  this->Alteration = AlterNormal;
  #if defined(BASIC_SOUND_DEBUG)
    Serial.begin(9600);
  #endif
}

TNotePlayer::TNotePlayer()
{
  _Init();
  this->_pwm_pin = SPEAKER;
  pinMode(this->_pwm_pin, OUTPUT);
}

TNotePlayer::TNotePlayer(int pin)
{
  _Init();
  if (pin < MAX_PIN) 
    this->_pwm_pin = pin;
  pinMode(this->_pwm_pin, OUTPUT);
}


