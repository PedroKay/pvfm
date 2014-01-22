
class pvfm_ui{

private:

    unsigned int make_color(unsigned char r, unsigned char g, unsigned char b);
    


public:

    void begin();
    
    bool isTouch();
    bool getTouchRect(int XL, int YU, int XR, int YD);
    
    unsigned char getTouchItem();
    
    void dispSetMode();

};


extern pvfm_ui UI;