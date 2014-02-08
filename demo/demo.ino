// pvfm demo

#include <Streaming.h>
#include <TFTv2.h>
#include <SPI.h>
#include <EEPROM.h>
#include <SeeedTouchScreen.h>
#include <MsTimer2.h>
#include <pvfm_dta.h>
#include <Wire.h>

#include "pvfm_ui.h"
#include "pvfm_ui_dfs.h"
//#include "pvfm_temp.h"


#define ADDR_I2C_SLAVE          19

// send a 't' to request temperature
void write_cmd_get_temp()
{
    Wire.beginTransmission(ADDR_I2C_SLAVE); // transmit to device #4
    Wire.write("t\r\n");        // sends five bytes             // sends one byte
    Wire.endTransmission();    // stop transmitting
}

void write_cmd_set_temp(int temp)
{
    if(temp<0 || temp>999)return;
    
    char str[10];
    if(temp<10)
    {
        sprintf(str, "s00%d\r\n", temp);
    }
    else if(temp<100)
    {
        sprintf(str, "s0%d\r\n", temp);
    }
    else
    {
        sprintf(str, "s%d\r\n", temp);
    }
    
    Wire.beginTransmission(ADDR_I2C_SLAVE);             // transmit to device #4
    Wire.write(str);        
    Wire.endTransmission(); 
    
    
    Wire.requestFrom(ADDR_I2C_SLAVE, 4);
    
    char dtaI2C[10];
    char dtaLen     = 0;
    
    while(1)
    {
    
        while(Wire.available())             // slave may send less than requested
        {
            char c = Wire.read();           // receive a byte as character
            Serial.print(c);                // print the character
            dtaI2C[dtaLen++] = c;
            if(dtaI2C[dtaLen-1] == '\n')
            {
                if(dtaI2C[0] == 'O' && dtaI2C[1] == 'K')
                {
                    cout << "set ok" << endl;
                    return;
                }
            }
        }
    }
    
}

bool getTemp_I2C(int *dta)
{
    write_cmd_get_temp();

    Wire.requestFrom(ADDR_I2C_SLAVE, 5);    // request 5 bytes from slave device #19

    char dtaI2C[10];
    char dtaLen     = 0;

    int cnt_tout = 0;
    while(1)
    {
        while(Wire.available())             // slave may send less than requested
        {
            char c = Wire.read();           // receive a byte as character
            Serial.print(c);                // print the character
            dtaI2C[dtaLen++] = c;
            if(dtaI2C[dtaLen-1] == '\n')
            {
                int sum = 0;
                sum += 100*(dtaI2C[0]-'0') + 10*(dtaI2C[1]-'0') + dtaI2C[2]-'0';
                *dta = sum;
                return 1;
            }
        }

        delay(1);
        cnt_tout++;
        if(cnt_tout>100)return 0;
    }
    return 0;
}

void setConfigValue(int item, int val)
{
    if(item == 0)                       // set temperature
    {
        write_cmd_set_temp(val);


    }
}

void setup()
{
    Serial.begin(115200);
    cout << "hello world" << endl;

    UI.begin();
    UI.normalPage();

    Wire.begin();                           // join i2c bus (address optional for master)
    
    
    write_cmd_set_temp(P_DTA.get_temps());
}

long timer1 = 0;




void loop()
{

    int item = UI.getTouchItem();

    if(item > 0)
    {
        int val_b = UI.getVal(item-1);
        val_b = UI.setNum(val_b, 10, 500);
        UI.setValue(val_b, item-1);
        
        setConfigValue(item-1, val_b);

        UI.normalPage();
    }

    if(millis()-timer1 > 200)
    {
        int tp;
        timer1 = millis();
        if(getTemp_I2C(&tp))
        {
            UI.setTempNow(tp);
            UI.updateTemp();
        }
    }


}