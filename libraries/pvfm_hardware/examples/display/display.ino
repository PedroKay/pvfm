// test of display
#include <pvfm_display.h>
#include <SerialLCD.h>
#include <SoftwareSerial.h>

PVFM_DISPLAY disp;

void setup()
{
    disp.init();
    disp.str(1, "hello");
}

void loop()
{
    
}