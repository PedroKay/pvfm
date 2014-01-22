
#ifndef __PVFM_DTA_H__
#define __PVFM_DTA_H__

#define ADDR_TEMP_SET   100
#define ADDR_TIME_1     102
#define ADDR_TIME_2     104

class pvfm_dta{

private:

    int temp_set;
    int temp_now;
    
    int time_1;
    int time_2;
    
    
    void write_word_eeprom(int addr, int dta);
    int get_word_eeprom(int addr);
    
public:

    void begin();
    void set_temps(int tp);                 // set temp_set
    void set_tempn(int tp);                 // set temp_now
    void set_time1(int tm);                 // set time1
    void set_time2(int tm);                 // set time2
    
    int get_temps(){return temp_set;}
    int get_tempn(){return temp_now;}
    int get_time1(){return time_1;}
    int get_time2(){return time_2;}

};

#endif