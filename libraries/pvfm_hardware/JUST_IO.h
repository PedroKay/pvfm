/*
  just_io.h
  
  Author:Loovee
  2013-1-20
  
  The MIT License (MIT)
  Copyright (c) 2013 Seeed Technology Inc.
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef __JUST_IO_H__
#define __JUST_IO_H__

class JUST_IO{


private:

    unsigned char __state;            // 
    int __pin;                      // pin
    int __onoff;                    // HIGH on or LOW on

public:
    
    void begin(int pin, int onState)
    {
        __state = 0;
        __pin   = pin;
        __onoff = onState;
        pinMode(pin, OUTPUT);
    }

    void on()
    {
        digitalWrite(__pin, __onoff);
        __state = 1;
    }
    
    void off()
    {
        digitalWrite(__pin, 1-__onoff);
        __state = 0;
    }
    
    void get()
    {
        return __state;
    }

};


#endif