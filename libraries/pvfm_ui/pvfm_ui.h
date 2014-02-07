
#ifndef __PVFM_UI_H__
#define __PVFM_UI_H__

#define __UIDBG     0

class pvfm_ui{

private:
   
    int num_set;
    

    int value[4];
    int temp_now_buf;
    
    
private:

    void dispSetMode();
    unsigned int make_color(unsigned char r, unsigned char g, unsigned char b);         // make rgb to 565 color
    
    bool isTouch();
    bool getTouchRect(int XL, int YU, int XR, int YD);                                  // if certain rect get touched
    
    
    void dispNum(int num, int num_buf, int x, int y, int size, int color, int color_bk);
    
    void updateValue();
    
public:

    void begin();
    unsigned int setNum(int num_input, int _min, int _max);                 // set number
    unsigned char getTouchItem();
    unsigned char updateTemp();                                             // refresh temperature
    
    void normalPage();                                                      // normal page
    void setTempNow(int tpn);                                               // set temperature now
    void setTempNow();                                                      // get temperature now from pvfm_dta
    
    void setValue(int val, int which_val);                                  // set value
    int getVal(int wh_val);                                                 // get value

};


extern pvfm_ui UI;

#endif