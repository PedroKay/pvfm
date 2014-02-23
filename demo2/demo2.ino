// pvfm demo

#include <Streaming.h>
#include <TFTv2.h>
#include <SPI.h>
#include <EEPROM.h>
#include <SeeedTouchScreen.h>
#include <MsTimer2.h>
#include <pvfm_dta.h>
#include <Wire.h>
#include <dbg_lvc.h>
#include <SD.h>

#include "pvfm_ui.h"
#include "pvfm_ui_dfs.h"

#define ADDR_I2C_SLAVE          19

#define isNum(num)              (num>='0' && num<='9')

#define __DebigIno              1                   // if print debug

// FBTYPE
#define CMD_TYPE_GET            0
#define CMD_TYPE_SET            1

// cmd return

#define CMD_RETURN_OK           0       // get data ok
#define CMD_RETURN_TIMEOUT      1       // timeout
#define CMD_RETURN_ERRINPUT     2
#define CMD_RETURN_ERRDATA      3
#define CMD_RETURN_TOOLONG      4

// some command
#define CMD_GET_TEMP            't'     // get temperature sensor from slave
#define CMD_SET_TEMP            's'     // set temperature
#define CMD_STEP_UP             'u'     // step move up
#define CMD_STEP_DOWN           'd'     // step move down


void printErr(int errCode)
{

#if __DebigIno

    if(!errCode)
    {
        cout << "cmd send OK" << endl;
        return;
    }
    cout << "ErrorCode: ";
    switch(errCode)
    {
        case CMD_RETURN_TIMEOUT:
        cout << "get data timeout" << endl;
        break;
        
        case CMD_RETURN_ERRINPUT:
        cout << "err input" << endl;
        break;
        
        case CMD_RETURN_ERRDATA:
        cout << "err data" << endl;
        break;
        
        case CMD_RETURN_TOOLONG:
        cout << "data too long" << endl;
        break;
        
        default:
        ;
    }
#endif
}

void pirntCmd(char cmd, int *dta, unsigned char cmdtype)
{
    
}

// write command and get feedback
// cmd - command
// *dta - data return if cmdtype = CMD_TYPE_GET
// cmdtype - feedback type, a num or "ok"
int write_cmd_get_fb(char cmd, int *dta, unsigned char cmdtype)
{

    if(dta == NULL)return CMD_RETURN_ERRINPUT;
    if(cmdtype != CMD_TYPE_GET && cmdtype != CMD_TYPE_SET)return CMD_RETURN_ERRINPUT;

    char dtaI2C[10];
    char dtaLen = 0;
    
    if(cmdtype == CMD_TYPE_SET)
    {
        int tmp = *dta;
        tmp = abs(tmp);

        if(*dta<10)
        {
            sprintf(dtaI2C, "%c00%d\r\n", cmd, *dta);
        }
        else if(*dta<100)
        {
            sprintf(dtaI2C, "%c0%d\r\n", cmd, *dta);
        }
        else if(*dta<1000)
        {
            sprintf(dtaI2C, "%c%d\r\n", cmd, *dta);
        }
        else return CMD_RETURN_ERRINPUT;
    }
    else if(cmdtype == CMD_TYPE_GET)
    {
        sprintf(dtaI2C, "%c\r\n", cmd);
    }

    Wire.beginTransmission(ADDR_I2C_SLAVE);
    Wire.write(dtaI2C);
    Wire.endTransmission();

    int request_num = (CMD_TYPE_SET == cmdtype) ? 4 : 5;
    Wire.requestFrom(ADDR_I2C_SLAVE, request_num);

    long timer_out = millis();

    while(1)
    {
        while(Wire.available())             // slave may send less than requested
        {
            char c = Wire.read();           // receive a byte as character
            Serial.print(c);                // print the character
            dtaI2C[dtaLen++] = c;

            if(dtaLen > 9) return CMD_RETURN_TOOLONG;

            if(dtaI2C[dtaLen-1] == '\n')
            {
                break;
            }
        }
        if(dtaI2C[dtaLen-1] == '\n')break;
        if(millis()-timer_out > 5000)return CMD_RETURN_TIMEOUT;          // time out
    }

    if(dtaLen != 4 && dtaLen != 5)return CMD_RETURN_ERRDATA;

    if(CMD_TYPE_SET == cmdtype && dtaLen == 4)
    {
        return (dtaI2C[0] == 'O' && dtaI2C[1] == 'K');
    }
    else if(CMD_TYPE_GET == cmdtype && dtaLen == 5)
    {
        if(isNum(dtaI2C[0]) && isNum(dtaI2C[1]) && isNum(dtaI2C[2]))
        {
            int sum = 0;
            sum += 100*(dtaI2C[0]-'0') + 10*(dtaI2C[1]-'0') + 1*(dtaI2C[2]-'0');
            *dta = sum;
            return CMD_RETURN_OK;
        }
        else return CMD_RETURN_ERRINPUT;
    }

    return -1;

}

void write_cmd_set_temp(int temp)
{
    int err = write_cmd_get_fb(CMD_SET_TEMP, &temp, CMD_TYPE_SET);
    printErr(err);
}

void write_cmd_step(int step)
{
    char __cmd = step>0 ? CMD_STEP_UP : CMD_STEP_DOWN;
    int __step = abs(step);
    
    int err = write_cmd_get_fb(__cmd, &__step, CMD_TYPE_SET);
    printErr(err);
}

void setConfigValue()
{
    write_cmd_set_temp(P_DTA.get_temps());
}

void setup()
{
    Serial.begin(115200);
    DBG.init();
    cout << "hello world" << endl;
    UI.begin();
    UI.normalPage();
    Wire.begin();                           // join i2c bus (address optional for master)
    
    
   // write_cmd_set_temp(P_DTA.get_temps());
    
    cout << "setup over" << endl;
}

long timer1 = 0;

void loop()
{

    DBG.timer_isr_dbg_lvc();
    
    // just debug, get a number from serial, and control the step
    if(DBG.isGetNum())
    {
        long num = 0;
        if(DBG.getNum(&num))
        {
            cout << "step = " << num << endl;
            write_cmd_step(num);
        }
        
       // setConfigValue();                 // when add i2c , it's needed
    }
    
    if(UI.isGotoModify())
    {   
        UI.modeModify();
    }
    
    
    if(millis() - timer1 > 200)
    {
        timer1 = millis();
        
        UI.setTempNow(random(0, 300));
        UI.updateTemp();
    }
 /*       
    // refresh temperature per 200ms
    if(millis()-timer1 > 200)
    {
        timer1 = millis();
        int tp;
        int err = write_cmd_get_fb(CMD_GET_TEMP, &tp, CMD_TYPE_GET);
        
        printErr(err);
        
        if(!err)            // get temperature and display
        {
            UI.setTempNow(tp);
            //UI.updateTemp();
        }
    }
*/
    
    /*
    int item = UI.getTouchItem();

    

    // get pressed
    if(item > 0)
    {
        int val_b = UI.getVal(item-1);
        val_b = UI.setNum(item-1, val_b, 10, 500);
        
        UI.setValue(val_b, item-1);
        setConfigValue(item-1, val_b);
        UI.normalPage();
    }

    // refresh temperature per 200ms
    if(millis()-timer1 > 200)
    {
        timer1 = millis();
        int tp;
        int err = write_cmd_get_fb(CMD_GET_TEMP, &tp, CMD_TYPE_GET);
        
        printErr(err);
        
        if(!err)            // get temperature and display
        {
            UI.setTempNow(tp);
            UI.updateTemp();
        }
    }

    // just debug, get a number from serial, and control the step
    if(DBG.isGetNum())
    {
        long num = 0;
        if(DBG.getNum(&num))
        {
            cout << "step = " << num << endl;
            write_cmd_step(num);
        }
    }*/
}    