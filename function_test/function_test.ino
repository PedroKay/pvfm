#include <Streaming.h>

int a = 0;

int fun_t()
{
    a++;
    
    cout << "a = " << a << endl;
    if(a>10)
    {
        a = 0;
        return 1;
    }
    else return 0;
}

void fun_p_test(int (*fun)())
{
    while(1)
    {
        if(fun())
        {
            cout << "exit now" << endl;
            return;
        }
        delay(100);
    }
}

void setup()
{
    Serial.begin(115200);
    cout << "hello world" << endl;
    
    fun_p_test(fun_t);
}

void loop()
{

}