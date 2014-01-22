
#ifndef __PVFM_UI_H__
#define __PVFM_UI_H__

class pvfm_ui{

private:

    
    
    int num_set;

    
public:

    void begin();
    
    unsigned int make_color(unsigned char r, unsigned char g, unsigned char b);
    
    bool isTouch();
    bool getTouchRect(int XL, int YU, int XR, int YD);
    
    unsigned char getTouchItem();
    
    void dispSetMode();
    
    unsigned int setNum(int num_input);
    
    
    void dispNum(int num, int num_buf, int x, int y, int size, int color, int color_bk);
};


extern pvfm_ui UI;

#endif