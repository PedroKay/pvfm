#include <Wire.h>
#include <Streaming.h>

#define RELAY_ADDR      0x59

const int pin_relay = 7;

char dtaI2c[20];
int dtaLen = 0;

bool dtaGet = 0;

void setup()
{
    Serial.begin(115200);
    
    Wire.begin(RELAY_ADDR);                     // join i2c bus with address #4
    Wire.onReceive(receiveEvent);               // register event
    
    pinMode(13, OUTPUT);
    pinMode(pin_relay, OUTPUT);
    
}

void loop()
{
    if(dtaGet)
    {
        dtaGet = 0;
        
        if(dtaI2c[0] == 'O' && dtaI2c[1] == 'O')
        {
            cout << "open" << endl;
            digitalWrite(13, HIGH);
            digitalWrite(pin_relay, HIGH);
        }
        else if(dtaI2c[0] == 'C' && dtaI2c[1] == 'C')
        {
            cout << "close" << endl;
            digitalWrite(13, LOW);
            digitalWrite(pin_relay, LOW);
        }
        
        dtaLen = 0;
    }
    
    delay(1);
}


void receiveEvent(int howMany)
{
    while(Wire.available())
    {
        dtaI2c[dtaLen++]= Wire.read();
        if(dtaI2c[dtaLen-1] == '\n')
        {
            dtaGet = 1;
        }
    }
}