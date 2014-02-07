#include <Arduino.h>
#include <seeed_pwm.h>
#include <stepper_4wd.h>
#include <BetterStepper.h>
#include <Streaming.h>

#define CMD_START    1
#define CMD_STOP     2
#define CMD_COUNT    3
#define CMD_DIST     4
#define CMD_SECTIONS 5
#define CMD_CUR_SEC  6
#define CMD_ACK      9

const int stepsPerRevolution = 200;
const int stepPerHole = 5; // = 9deg/1.8deg, 9deg = 360deg/40teeth

// initialize the stepper library on pins 8 through 11:
stepper_4wd stepperCut(stepsPerRevolution);
//BetterStepper stepperPush(stepsPerRevolution, 2,11,12,13);


unsigned int partDist  = 1;       //how many holes per part?
unsigned int partCount = 20;     //how many part to cut?
unsigned int sectionCount = 0;

boolean running = false;
boolean ctrlStopped = true;

void send_msg(unsigned char cmd, unsigned int msg);
boolean read_msg(unsigned char *cmd, unsigned int *msg);




void setup() {
    // initialize the serial port:
    Serial.begin(9600);
    Serial.setTimeout(10);

    cout << "hello world" << endl;

    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);

    analogWrite(3, 100);

    // pwm set
    PWM.init(); //FREQPWM


    PWM.setPwm(9, 1, FREQPWM);
    PWM.setPwm(10, 1, FREQPWM);

    stepperCut.setSpeed(100, 200);
}

int previous = 0;

int val_buf = 0;

int speed = 0;
int speed_buf = 0;

int dtaLen = 0;
int dtaUart[10];
int getDta = 0;


void move(int __step)
{
    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
    stepperCut.step(__step);

    PWM.setPwm(9, 1, FREQPWM);
    PWM.setPwm(10, 1, FREQPWM);
}

void loop()
{

    if(getDta)
    {
        getDta      = 0;
        dtaLen      = 0;
        int step_   = 0;
        if(dtaUart[0] == '-')
        {
            step_ = (dtaUart[1]-'0')*100 + (dtaUart[2]-'0')*10 + (dtaUart[3]-'0');
            step_ = 0-step_;
        }
        else
        {
            step_ = (dtaUart[0]-'0')*100 + (dtaUart[1]-'0')*10 + (dtaUart[2]-'0');
        }

        cout << step_ << endl;

        move(step_);

        cout << "run over" << endl;
    }

    delay(1);
}

void serialEvent()
{

    while (Serial.available())
    {
        // get the new byte:
        char inChar = (char)Serial.read();
        dtaUart[dtaLen++] = inChar;

        if(dtaUart[dtaLen-1] == '\n')
        {
            getDta = 1;
        }
    }
}


