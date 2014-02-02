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
    
    UI.normalPage();
 
}

long timer_tmp = 0;

int st_buf = 0;

int st_buf_ = 0;
void loop()
{

    //int a = UI.setNum(50, 40, 60);
    
   // cout << "num set to = " << a << endl;
   
    
    int item = UI.getTouchItem();
    
    if(item > 0)
    {
        int val_b = UI.getVal(item-1);
        val_b = UI.setNum(val_b, 10, 500);
        UI.setValue(val_b, item-1);
        UI.normalPage();
    }
}