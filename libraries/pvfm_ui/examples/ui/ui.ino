#include <Streaming.h>
#include <TFTv2.h>
#include <SPI.h>
#include <SeeedTouchScreen.h>

#include "pvfm_ui.h"
#include "pvfm_ui_dfs.h"

void setup()
{
    Serial.begin(115200);
    cout << "hello world" << endl;
    
    UI.begin();
    
    
    UI.dispSetMode();
}

long timer_tmp = 0;

int st_buf = 0;
void loop()
{

    st_buf = UI.getTouchItem();
    if(st_buf)
    {
        timer_tmp = millis();
        cout << st_buf << endl;
        
        for(;;)
        {
            if(st_buf == UI.getTouchItem())
            {
                timer_tmp = millis();
                delay(1);
            }
            
            if(millis()-timer_tmp > 100)break;
        }
    }
}