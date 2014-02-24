// find temp
#include <Streaming.h>

const int __PIN_K0 = A0;
const int __PIN_K1 = A2;
const int __PIN_K2 = A3;

const  int __PIN_SSR0   = 11;
const  int __PIN_SSR1   = 12;
const  int __PIN_SSR2   = 13;


void setup()
{
    Serial.begin(115200);
    
    cout << "hello world" << endl;
    
    
    pinMode(__PIN_SSR0, OUTPUT);
    pinMode(__PIN_SSR1, OUTPUT);
    pinMode(__PIN_SSR2, OUTPUT);
    
    digitalWrite(__PIN_SSR0, LOW);
    digitalWrite(__PIN_SSR1, LOW);
    digitalWrite(__PIN_SSR1, LOW);
    
    
    digitalWrite(__PIN_SSR0, HIGH);
}


void loop()
{
    cout << analogRead(__PIN_K0) << '\t' << analogRead(__PIN_K1) << '\t' << analogRead(__PIN_K2) << endl;
    
    delay(200);
}