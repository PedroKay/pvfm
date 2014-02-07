
#ifndef __PVFM_DTA_H__
#define __PVFM_DTA_H__

#include <EEPROM.h>

#define ADDR_TEMP_SET   100
#define ADDR_TIME_1     102
#define ADDR_TIME_2     104

class pvfm_dta{

private:
    
    int temp_now;
    
    void write_word_eeprom(int addr, unsigned int dta);
    unsigned int get_word_eeprom(int addr){return (unsigned int)EEPROM.read(addr)<<8 | EEPROM.read(addr+1);}
    
public:

    pvfm_dta(){temp_now = 0;}
    
    void set_temps(int tp){write_word_eeprom(100, tp);}                       // set temp_set
    void set_tempn(int tp){temp_now = tp;}                                               // set temp_now
    void set_time1(int tm){write_word_eeprom(102, tm);}                 // set time1
    void set_time2(int tm){write_word_eeprom(104, tm);}                 // set time2
    
    int get_temps(){return get_word_eeprom(100);}
    int get_tempn(){return temp_now;}
    int get_time1(){return get_word_eeprom(102);}
    int get_time2(){return get_word_eeprom(104);}

};

extern pvfm_dta P_DTA;

#endif