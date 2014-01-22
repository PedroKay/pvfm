// test of display
#include <Streaming.h>
#include <SerialLCD.h>
#include <SoftwareSerial.h>

#include <pvfm_display.h>
#include <pvfm_slide.h>


PVFM_DISPLAY disp;
PVFM_Slide slide;

PVFM_SSR ssr1(PINSSR1);
PVFM_SSR ssr2(PINSSR2);
PVFM_SSR ssr3(PINSSR3);


int val         = 0;
int val_buf     = 0;

const int numReadings   = 32;
const int numReadings_2 = 5;                // pow(2, 6) = 64

float temp_set = 400.0;


int readings[numReadings];                  // the readings from the analog input

int index       = 0;                        // the index of the current reading
long total      = 0;                        // the running total
int average     = 0;                        // the average
int average_buf = 0;


void initDta()
{
    for(int i=0; i<numReadings; i++)
    {
        readings[i] = 0;
    }
}


int pushDta(int dta)
{

    total= total - readings[index];         
    // read from the sensor:
    
    readings[index] = dta;

    total = total + readings[index];   

    index = index + 1;          

    if (index >= numReadings)              
    index = 0;                           
 
    average = (total >> numReadings_2);
    

    return average;

}



void setup()
{
    Serial.begin(115200);
    disp.init();
    cout << "hello world" << endl;
}

void loop()
{

    val = slide.getTempSet();

    if(val != val_buf)
    {
        val_buf = val;
        disp.num(0, val);
    }
    
    delay(10);
}