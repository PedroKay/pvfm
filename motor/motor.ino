#include <Stepper.h>

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8,11,12,13);

void setup() {
    // set the speed at 60 rpm:
    myStepper.setSpeed(60);
    // initialize the serial port:
    Serial.begin(9600);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    digitalWrite(9,HIGH);
    digitalWrite(10,HIGH);
}

void loop() {
    // step one revolution  in one direction:
    Serial.println("clockwise");
    myStepper.step(stepsPerRevolution);
    delay(500);

    //step one revolution in the other direction:
    Serial.println("counterclockwise");
    myStepper.step(-stepsPerRevolution);
    delay(500);
}
