/*
  pvfm_ssr.h

  Author:Loovee
  2013-12-26
 
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

#ifndef __PVFM_DISPLAY_H__
#define __PVFM_DISPLAY_H__

#include <Arduino.h>
#include <SerialLCD.h>
#include <SoftwareSerial.h>

#define PIN_RX          3
#define PIN_TX          4

SerialLCD slcd(PIN_RX, PIN_TX);

class PVFM_DISPLAY{

private:

public:

    void init()
    {
        slcd.begin();
    }
    
    // row = 0 or 1
    void num(int row, int num)
    {
        slcd.setCursor(0, row);
        slcd.printNum(num);
    }
    
    
    // row = 0 or 1
    void str(int row, char *str)
    {
        slcd.setCursor(0, row);
        slcd.print(str);
    }
    
    // clear
    void clear()
    {
        slcd.clear();
    }

};

#endif
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/