//

#include <Streaming.h>
#include "pvfm_temp_new.h"

PVFM_Temp_new tp(A5, A4);

void setup()
{
    Serial.begin(115200);
    
    cout << "hello world" << endl;
}


long timer1 = 0;
void loop()
{
    tp.pushDta();
    delay(1);
    
    if((millis() - timer1) > 200)
    {
        timer1 = millis();
        cout << tp.get_kt() << '\t' << tp.get_nt() << endl;
    }
}