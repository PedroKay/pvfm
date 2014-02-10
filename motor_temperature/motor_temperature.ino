// control motor and temperature
#include <MsTimer2.h>
#include <Streaming.h>
#include <EEPROM.h>
#include <Wire.h>

#include <seeed_pwm.h>
#include <stepper_4wd.h>
#include <BetterStepper.h>

#include "pvfm_temp.h"

#define ADDR_I2C_SLAVE          19


const int stepsPerRevolution = 200;
const int stepPerHole = 5; // = 9deg/1.8deg, 9deg = 360deg/40teeth

// initialize the stepper library on pins 8 through 11:
stepper_4wd stepperCut(stepsPerRevolution);


char dtaI2C[20];
int dtaLen  = 0;
bool dtaGet = 0;


void step_motor_init()
{
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    
    // pwm set
    PWM.init(); //FREQPWM


    PWM.setPwm(9, 1, FREQPWM);
    PWM.setPwm(10, 1, FREQPWM);

    stepperCut.setSpeed(100, 200);
}

void move(int __step)
{
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
    stepperCut.step(__step);

    PWM.setPwm(9, 1, FREQPWM);
    PWM.setPwm(10, 1, FREQPWM);
}

void setup()
{
    Serial.begin(115200);

    cout << "hello world" << endl;

    ptp.begin();
    ptp.setTemp(100);
    
    step_motor_init();

    Wire.begin(ADDR_I2C_SLAVE);                         // join i2c bus with address #4
    Wire.onReceive(receiveEvent);                       // register event
    Wire.onRequest(requestEvent);                       // register event
}

long timer1 = 0;


void loop()
{
    delay(1);

}

void receiveEvent(int howMany)
{
    while(0 < Wire.available()) // loop through all but the last
    {
        dtaI2C[dtaLen++] = Wire.read();
        if(dtaI2C[dtaLen-1] == '\n')
        {
            dtaGet = 1;
        }
        
        Serial.write(dtaI2C[dtaLen-1]);
    }
}


bool isNum(char c)
{
    return (c>='0' && 'c<=9');
}

int calcNumFromStr(char *dta)
{

    //if(NULL == dta)return -9999;
    
    //if(!isNum(dta[0]) || !isNum(dta[1]) || !isNum(dta[2]))return -9998;
    
    int sum = 0;
    sum = 100*(dta[0]-'0') + 10*(dta[1]-'0') + 1*(dta[2] - '0');
    return sum;
    
}

void requestEvent()
{

    if(dtaGet)
    {
        if(dtaI2C[0] == 't')
        {
            char str_[10];
            int t  = ptp.get_kt(0);
            int t1 = ptp.get_kt(1);
            int t2 = ptp.get_kt(2);
            
            cout << t << '\t' << t1 << '\t' << t2 << endl;
            
            //cout << "temperature = " << t << endl;
            if(t<10)
            sprintf(str_, "00%d\r\n", t);
            else if(t<100)
            sprintf(str_, "0%d\r\n", t);
            else
            sprintf(str_, "%d\r\n", t);

            Wire.write(str_);
            //Serial.print(str_);
        }
        else if( dtaI2C[0] == 's' && dtaLen == 6 )
        {
            int num = 100*(dtaI2C[1]-'0') + 10*(dtaI2C[2]-'0') + 1*(dtaI2C[3]-'0');
            
            cout << "set temperature: " << num << endl;
            ptp.setTemp(num);
            
            Wire.write("OK\r\n");
        }
        else if(dtaI2C[0] == 'u')
        {
            int steps = calcNumFromStr(&dtaI2C[1]);
            
            Serial.write(dtaI2C);
            cout << "step move up " << steps << "steps" << endl;
            Wire.write("OK\r\n");
            
            move(steps);
        }
        else if(dtaI2C[0] == 'd')
        {
            int steps = calcNumFromStr(&dtaI2C[1]);
            
            Serial.write(dtaI2C);
            cout << "step move down " << steps << "steps" << endl;
            Wire.write("OK\r\n");
            
            move(steps*-1);
        }
        dtaGet = 0;
        dtaLen = 0;
    }
}