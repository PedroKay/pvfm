// test of display
#include <Streaming.h>
#include <SerialLCD.h>
#include <SoftwareSerial.h>

#include <pvfm_display.h>
#include <pvfm_slide.h>
#include <pvfm_temp.h>
#include <pvfm_ssr.h>

PVFM_Temp temperature(PINTEMP3);
PVFM_DISPLAY disp;
PVFM_Slide slide;


PVFM_SSR ssr1(PINSSR1);
PVFM_SSR ssr2(PINSSR2);
PVFM_SSR ssr3(PINSSR3);


long timer1 = 0;
long timer_checkSlide   = 0;
long timer_dispShowTemp = 0;

int temp_set        = 0;
int temp_set_buf    = 0;


int ssrstate = 0;



float getTemperature()
{
    // ssr off
    
    float temp = temperature.get();
    
    if((temp_set-temp)<5)
    {
        int tmp = ssrstate;
        
        ssr1.off();
        delay(5);
        
        for(int i=0; i<32; i++)
        {
            temperature.pushDta();
        }

        temp = temperature.get();

        if(tmp)ssr1.on();
    }
    return temp;
    
}


void setup()
{
    Serial.begin(115200);
    disp.init();
    cout << "hello world" << endl;

}

int temp_int        = 0;
int temp_int_buf    = 0;


void loop()
{

    temperature.pushDta();
    
    if(millis() - timer_checkSlide > 100)
    {
        timer_checkSlide = millis();
        
        temp_set = slide.getTempSet();
        
        if(temp_set != temp_set_buf)
        {
            temp_set_buf = temp_set;
            disp.num(0, temp_set);
            
            cout << "temp set: " << temp_set << endl;
        }
    }
    
    if(millis()-timer1 > 100)
    {
        timer1 = millis();
        
        float temp_now = getTemperature();
        
        temp_int = (int)temp_now;

        if(ssrstate == 0 && temp_now <= (temp_set-0.2))
        {
            ssr1.on();
            ssrstate = 1;
        }
        else if(ssrstate == 1 && temp_now >= (temp_set+0.5))
        {
            ssr1.off();
            ssrstate = 0;
        }
        else if(temp_now > (temp_set+5))
        {
            ssr1.off();
            ssrstate = 0;
        }
        else if(temp_now < (temp_set-5))
        {
            ssr1.on();
            ssrstate = 0;
        }
        
        if(millis() - timer_dispShowTemp > 500 && temp_int != temp_int_buf)
        {
            timer_dispShowTemp = millis();
            temp_int_buf = temp_int;
            disp.num(1, temp_int);
        }
        
    }
    
    delay(1);
}