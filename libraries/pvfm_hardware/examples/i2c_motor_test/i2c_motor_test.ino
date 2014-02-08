#include <Wire.h>
#include <pvfm_i2c_motor.h>
#include <Streaming.h>
#include <EEPROM.h>
#include <MsTimer2.h>

void setup()
{
    Serial.begin(115200);
    cout << "hello world" << endl;
    
    cout << __LINE__ << endl;
    cout << __LINE__ << endl;
    
    Wire.begin();
    SuctionValve.init();
}

void loop()
{
    SuctionValve.run();
    delay(2000);
    SuctionValve.stop();
    delay(2000);
}
