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

int st_buf_ = 0;
void loop()
{

    UI.setNum(50);

}