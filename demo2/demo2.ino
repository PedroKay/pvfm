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
#include <pvfm_i2c_motor.h>


#include "pvfm_ui.h"
#include "pvfm_ui_dfs.h"

#define ADDR_I2C_SLAVE          19

#define isNum(num)              (num>='0' && num<='9')

#define __DebigIno              1                   // if print debug
#define __DBG_CMD               0                   // cmd dbg information

// FBTYPE
#define CMD_TYPE_GET            0
#define CMD_TYPE_SET            1

// cmd return

#define CMD_RETURN_OK           0       // get data ok
#define CMD_RETURN_TIMEOUT      1       // timeout
#define CMD_RETURN_ERRINPUT     2
#define CMD_RETURN_ERRDATA      3
#define CMD_RETURN_TOOLONG      4
#define CMD_UNKNOWN_ERR         5

// some command
#define CMD_GET_TEMP            't'     // get temperature sensor from slave
#define CMD_SET_TEMP            's'     // set temperature
#define CMD_STEP_UP             'u'     // step move up
#define CMD_STEP_DOWN           'd'     // step move down


// pin of button
#define PIN_LEFT    3
#define PIN_RIGHT   2

void btn_pin_init()
{
    pinMode(PIN_LEFT, INPUT);
    digitalWrite(PIN_LEFT, HIGH);
    
    pinMode(PIN_RIGHT, INPUT);
    digitalWrite(PIN_RIGHT, HIGH);
}

bool btn_left_read()
{
    return (0 == digitalRead(PIN_LEFT));
}

bool btn_right_read()
{
    return (0 == digitalRead(PIN_RIGHT));
}



#define STEP_ST_UP      0
#define STEP_ST_DOWN    1

#define MOTORTM_ON      0
#define MOTORTM_OFF     1

int state_step = STEP_ST_UP;
int state_motor_tm = MOTORTM_OFF;

void btn_fun()
{
    if(btn_left_read())
    {
        delay(10);
        if(btn_left_read())
        {
            cout << "btn left" << endl;
            
            if(state_step)
            {
                write_cmd_step(-1);
            }
            else
            {
                write_cmd_step(1);
            }
            
            state_step = 1-state_step;
            
            //cout << "write data finish" << endl;
            while(btn_left_read())
            {
                delay(5);
            }
            
            //cout << "btn out" << endl;
        }
    }
    
    if(btn_right_read())
    {
        delay(10);
        if(btn_right_read())
        {
            cout << "btn right" << endl;
            
            state_motor_tm = 1-state_motor_tm;
            
            if(state_motor_tm)
            {
                SuctionValve.run();
            }
            else
            {
                SuctionValve.stop();
            }
            
            while(btn_right_read())
            {
                delay(5);
            }
        }
    }
    
}

void printErr(int errCode)
{

#if __DBG_CMD

    if(CMD_RETURN_OK == errCode)
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
        
        case CMD_UNKNOWN_ERR:
        cout << "unknown error!" << endl;
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
    int dtaLen = 0;
    
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
            // Serial.print(c);                // print the character
            dtaI2C[dtaLen++] = c;

            if(dtaLen > 9) return CMD_RETURN_TOOLONG;

            if(dtaI2C[dtaLen-1] == '\n')
            {
                break;
            }

        }
        if(dtaI2C[dtaLen-1] == '\n')
        {
            // cout << "get data from i2c: " << dtaLen << endl;
            // cout << "break" << endl;
            break;
        }
        
        if(millis()-timer_out > 1000)       // time out
        {
            // cout << "tout" << endl;
            return CMD_RETURN_TIMEOUT;  
        }
    }

    if(dtaLen != 4 && dtaLen != 5)return CMD_RETURN_ERRDATA;

    if(CMD_TYPE_SET == cmdtype && dtaLen == 4)
    {
        return ((dtaI2C[0] == 'O' && dtaI2C[1] == 'K') ? CMD_RETURN_OK : CMD_RETURN_ERRDATA);
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

    return CMD_UNKNOWN_ERR;

}

void write_cmd_set_temp(int temp)
{
    int err = write_cmd_get_fb(CMD_SET_TEMP, &temp, CMD_TYPE_SET);
    printErr(err);
    
    
    if(CMD_RETURN_OK == err)
    {
        cout << "set temperature OK" << endl;
    }
    else
    {
        cout << "set temperature Nok" << endl;
    }
    
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
    
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);

    UI.begin();
    UI.normalPage();
    Wire.begin();                           // join i2c bus (address optional for master)
    
    write_cmd_set_temp(P_DTA.get_temps());
    
    SuctionValve.init();
    
    cout << "setup over" << endl;
}

long timer1 = 0;

void loop()
{

    DBG.timer_isr_dbg_lvc();
    
    // input a num to control the stepper, num must between 0-999
    // such as input 100 means stepper move up 100 steps while -100 move down 100 steps
    
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
    
    // if goto modofy mode
    if(UI.isGotoModify())
    {   
        UI.modeModify();
        
        setConfigValue();                 // when add i2c , it's needed
    }
    
    
    btn_fun();
    
    
    if(millis() - timer1 > 500)
    {
    
    
#if 0
        timer1 = millis();
        
        UI.setTempNow(random(0, 300));
        UI.updateTemp();
#else
        timer1 = millis();
        int tp;
        int err = write_cmd_get_fb(CMD_GET_TEMP, &tp, CMD_TYPE_GET);
        
        printErr(err);
        
        if(!err)            // get temperature and display
        {
            UI.setTempNow(tp);
            UI.updateTemp();
        }
#endif    
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