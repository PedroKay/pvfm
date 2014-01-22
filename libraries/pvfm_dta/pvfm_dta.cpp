
#include <EEPROM.h>

#include "pvfm_dta.h"


void pvfm_dta::write_word_eeprom(int addr, int dta)
{
    EEPROM.write(addr, dta>>8);
    EEPROM.write(addr+1, dta);
}

unsigned int pvfm_dta::get_word_eeprom(int addr)
{
    unsigned int tmp = EEPROM.read(addr);
    tmp = tmp<<8 + EEPROM.read(addr+1);
    return tmp;
}


void pvfm_dta::begin()
{
    temp_set = get_word_eeprom(ADDR_TEMP_SET);
    temp_now = 0;
    
    time_1   = get_word_eeprom(ADDR_TIME_1);
    time_2   = get_word_eeprom(ADDR_TIME_2);
}

void pvfm_dta::set_temps(int tp)                 // set temp_set
{
    temp_set = tp;
    write_word_eeprom(ADDR_TEMP_SET, temp_set);
}

void pvfm_dta::set_tempn(int tp)                 // set temp_now
{
    temp_now = tp;
}

void pvfm_dta::set_time1(int tm)                 // set time1
{
    time_1 = tm;
    write_word_eeprom(ADDR_TIME_1, time_1);
}

void pvfm_dta::set_time2(int tm)                 // set time2
{
    time_2 = tm;
    write_word_eeprom(ADDR_TIME_2, time_2);
}

int get_time2();pvfm_dta::