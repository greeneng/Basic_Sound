/*
 * Example of using library "basic_sound"
 * plays "Imperial March" and "Jingle Bells" in Basic format
 */
#include "basic_sound.h"

TNotePlayer nplayer(2);

void setup()
{
 //Serial.begin(9600);
 TNotePlayer nplayer(2);

 char* str1 = "T100 M- MS L4 O6 GGGE8.B16 GE8.B16G2 >DDDE8.<B16 F#E8.B16G2 \0";
 char* str2 = "O5 G<G8.G16>GF#8.F'16 E'16E+16E'8P8<A-8>D-C8.<B'16 O4 B16A16B8P8E8F#E8.F16 BG8.B16>D2\0";
 char* strn = "t200L4o4mneel2el4eel2el4egl3cl8dl1el4ffl3fl8fl4fel2el8eel4eddel2dgl4eel2el4eel2el4egl3cl8dl1el4ffl3fl8fl4fel2el8efl4ggfdl2c\0";
 nplayer.playString(str1);
 nplayer.playString(str2);
 nplayer.playString(str2);
 delay(2000);
 nplayer.playString(strn);
}
 
void loop()
{
 // Нажмите кнопку сброса(Reset) для повторного воспроизведения.
 // Press <Reset> button to repeat
}
