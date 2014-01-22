
#include <Streaming.h>
#include <SeeedTouchScreen.h>
#include <TFTv2.h>
#include <SPI.h>


#include "pvfm_ui.h"
#include "pvfm_ui_dfs.h"

TouchScreen ts = TouchScreen(XP, YP, XM, YM);

void pvfm_ui::begin()
{
    TFT_BL_ON;          // turn on the background light
    Tft.TFTinit();      // init TFT library
    
   /*
    int color_g = make_color(COLOR_TEMP_SET_R, COLOR_TEMP_SET_G, COLOR_TEMP_SET_B);
    Tft.fillRectangle(LOCA_TEMP_SET_X, LOCA_TEMP_SET_Y, 240, 64, color_g);
    
    color_g = make_color(COLOR_TEMP_NOW_R, COLOR_TEMP_NOW_G, COLOR_TEMP_NOW_B);
    Tft.fillRectangle(LOCA_TEMP_NOW_X, LOCA_TEMP_NOW_Y, 240, 64, color_g);
    
    color_g = make_color(COLOR_TIME1_R, COLOR_TIME1_G, COLOR_TIME1_B);
    Tft.fillRectangle(LOCA_TIME1_X, LOCA_TIME1_Y, 240, 64, color_g);
    
    color_g = make_color(COLOR_TIME2_R, COLOR_TIME2_G, COLOR_TIME2_B);
    Tft.fillRectangle(LOCA_TIME2_X, LOCA_TIME2_Y, 240, 64, color_g);
    
    color_g = make_color(COLOR_STATUS_R, COLOR_STATUS_G, COLOR_STATUS_B);
    Tft.fillRectangle(LOCA_STATUS_X, LOCA_STATUS_Y, 240, 64, color_g);
*/
}

unsigned int pvfm_ui::make_color(unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int color_ = r;
    
    color_ = r>>3;                              // red
    color_ = color_<<6 | (g>>2);                // green
    color_ = color_<<5 | (b>>3);                // blue
    
    return color_;           
}

bool pvfm_ui::isTouch()
{
    Point p = ts.getPoint();
    if(p.z > __PRESURE)return 1;
    else return 0;
}

bool pvfm_ui::getTouchRect(int XL, int YU, int XR, int YD)
{
    if(!isTouch())return 0;
    Point p = ts.getPoint();
    
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
    
    if(p.z < __PRESURE)return 0;
    
    if(p.x > XL && p.x < XR && p.y > YU && p.y < YD)return 1;
    return 0;
}

unsigned char pvfm_ui::getTouchItem()
{
    if(!isTouch())return 0;
    
    Point p = ts.getPoint();
    
    p.x = map(p.x, TS_MINX, TS_MAXX, 0, 240);
    p.y = map(p.y, TS_MINY, TS_MAXY, 0, 320);
    
    if(p.z < __PRESURE)return 0;
    

    if(p.y < LOCA_TEMP_NOW_Y)
    {
        return 1;
    }
    else if(p.y < LOCA_TIME1_Y)
    {
        return 2;
    }
    else if(p.y < LOCA_TIME2_Y)
    {
        return 3;
    }
    else if(p.y < LOCA_STATUS_Y)
    {
        return 4;
    }
    else return 5;

    
}

void pvfm_ui::dispSetMode()
{
    int x_t  = 60;
    int y_t  = 120;
    
    int h_t  = 50;
    
    int len_r = 40;
    int wid_r = 50;
    
    for(int i=0; i<h_t; i++)
    {
        Tft.drawHorizontalLine(x_t-i, y_t+i, 2*i+1, make_color(255, 0, 0));
        
        Tft.drawHorizontalLine(240-x_t-i, y_t+h_t+wid_r-i, 2*i+1, make_color(255, 0, 0));
    }
    
    Tft.fillRectangle(x_t-(len_r/2), y_t+h_t, len_r, wid_r, make_color(255, 0, 0));
    
 
}


pvfm_ui UI;