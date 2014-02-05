#include <MsTimer2.h>

#include "pvfm_temp.h"

#define __PIN_K     A0
#define __PIN_N     A1

float Var_VtoT_K[3][10]=
{
    {0, 2.5173462e1, -1.1662878, -1.0833638, -8.9773540/1e1, -3.7342377/1e1,
    -8.6632643/1e2, -1.0450598/1e2, -5.1920577/1e4},
    {0, 2.508355e1, 7.860106/1e2, -2.503131/1e1, 8.315270/1e2,
    -1.228034/1e2, 9.804036/1e4, -4.413030/1e5, 1.057734/1e6, -1.052755/1e8},
    {-1.318058e2, 4.830222e1, -1.646031, 5.464731/1e2, -9.650715/1e4,
    8.802193/1e6, -3.110810/1e8}
};

// Corresponding analog value of 50-550, per 1oC. such as xxx[0] means analogValue of 50,
// xxx[10] means analogValue of 50+5*10 = 100oC

float val_T2V[101] =
{
    2.02,    2.23,    2.44,    2.64,    2.85,    3.06,    3.27,    3.47,    3.68,    3.89,          // 50 - 95
    4.10,    4.30,    4.51,    4.71,    4.92,    5.12,    5.33,    5.53,    5.73,    5.94,          // 100 - 145
    6.14,    6.34,    6.54,    6.74,    6.94,    7.14,    7.34,    7.54,    7.74,    7.94,          // 150 - 195
    8.14,    8.34,    8.54,    8.74,    8.94,    9.14,    9.34,    9.54,    9.75,    9.95,          // 200 - 245
    10.15,   10.36,   10.56,   10.77,   10.97,   11.18,   11.38,   11.59,   11.79,   12.00,         // 250 - 295
    12.21,   12.42,   12.62,   12.83,   13.04,   13.25,   13.46,   13.67,   13.87,   14.08,         // 300 - 345
    14.29,   14.50,   14.71,   14.92,   15.13,   15.34,   15.55,   15.76,   15.98,   16.19,         // 350 - 395
    16.40,   16.61,   16.82,   17.03,   17.24,   17.45,   17.67,   17.88,   18.09,   18.30,         // 400 - 445
    18.52,   18.73,   18.94,   19.15,   19.37,   19.58,   19.79,   20.01,   20.22,   20.43,         // 450 - 495
    20.64,   20.86,   21.07,   21.28,   21.50,   21.71,   21.92,   22.14,   22.35,   22.56,         // 500 - 545
    22.77,                                                                                          // 550
};

unsigned int temp_2_analog[101] = 
{
    79,	    87,	    95,	    103,	111,	120,	128,	136,	144,	152,				// 50 - 95
    161,	168,	177,	185,	193,	201,	209,	217,	225,	233,				// 100 - 145
    241,	249,	256,	264,	272,	280,	288,	296,	304,	311,				// 150 - 195
    319,	327,	335,	343,	351,	359,	366,	374,	383,	390,				// 200 - 245
    398,	406,	414,	423,	430,	439,	447,	455,	463,	471,				// 250 - 295
    479,	487,	495,	504,	512,	520,	528,	537,	544,	553,				// 300 - 345
    561,	569,	577,	586,	594,	602,	610,	619,	627,	635,				// 350 - 395
    644,	652,	660,	668,	677,	685,	694,	702,	710,	718,				// 400 - 445
    727,	735,	744,	752,	760,	769,	777,	786,	794,	802,				// 450 - 495
    810,	819,	827,	835,	844,	852,	861,	869,	877,	886,				// 500 - 545
894
};

void timer3_isr()
{
    ptp.__timer_isr();
}

// 5ms
void PVFM_Temp::__timer_isr()
{
    pushDta();
    
    if(flg_heat == 0 && average < (temp_set_2-3))
    {
        flg_heat = 1;
        
        cout << "average = " << average << '\t';
        cout << "temp_set_2 = " << temp_set_2 << endl;
        
        Serial.println("ssr on");
    }
    else if(flg_heat == 1 && average > (temp_set_2+3))
    {
        flg_heat = 0;
        cout << "average = " << average << '\t';
        cout << "temp_set_2 = " << temp_set_2 << endl;
        
        Serial.println("ssr off");
    }
}

void PVFM_Temp::begin()
{
    __pin_k = __PIN_K;
    __pin_n = __PIN_N;
    

    index       = 0;                        // the index of the current reading
    total       = 0;                        // the running total
    average     = 0;                        // the average
    average_buf = 0;

    temp_set    = 0;                        // default value: 0oC
    temp_set_2  = temp_2_analog[0];

    initDta();

    __temp_n = 0;
    
    for(int i=0; i<32; i++)
    {
        __temp_n += get_nt();
    }
    
    __temp_n = __temp_n>>5;
    
    makeArray();
    
    cout << "__temp_nxxx = " << __temp_n << endl;
    
    flg_heat    = 0;

    MsTimer2::set(5, timer3_isr); // 500ms period
    MsTimer2::start();
}

void PVFM_Temp::setTemp(int tpr)                           // set temperature
{
    temp_set    = tpr;
    temp_set_2  = (tpr-50)/5;
    temp_set_2  = temp_2_analog[temp_set_2];
}


void PVFM_Temp::makeArray()
{

    float __Vref    = 5.0;

    for(int i=0; i<101; i++)
    {
        val_T2V[i] -= __temp_n*0.040295;
    }
    
    for(int i=0; i<101; i++)
    {
        temp_2_analog[i] = val_T2V[i]/1000*Av_Amplifer*1023.0/__Vref;
    }

    for(int i=0; i<10; i++)
    {
        for(int j=0; j<10; j++)
        {
            cout << temp_2_analog[i*10+j] << ",\t";
        }
        cout << "\t\t\t// " << 50*i+50 << " - " << 50*i+95 << endl;
    }

    cout << temp_2_analog[100] << endl;
}

void PVFM_Temp::getArray()
{

}

float PVFM_Temp::K_VtoT(float mV)
{
    unsigned char i = 0;
    float value = 0;

    if(mV >= 0 && mV < 20.644)          // 20.644
    {
        value = Var_VtoT_K[1][9];

        for(i = 9; i >0; i--)
        value = mV * value + Var_VtoT_K[1][i-1];
    }
    else if(mV >= 20.644 && mV <= 54.900)
    {
        value = Var_VtoT_K[2][6];
        for(i = 6; i >0; i--)
        value = mV * value + Var_VtoT_K[2][i-1];
    }

    return value;
}


// get analog data
int PVFM_Temp::getAnalog(int pin)                         // return button state, HIGH or LOW, -1: pin err
{
    int sum = 0;
    for(int i=0; i<8; i++)
    {
        sum += analogRead(pin);
    }

    return sum >> 3;
}

// init buff
void PVFM_Temp::initDta()
{
    for(int i=0; i<numReadings; i++)
    {
        readings[i] = 0;
    }
}


// return temperature
float PVFM_Temp::get_kt()
{

    // float vol = (float)average/1023.0*5000.0 - BiasVol;

    float vol = (float)average/1023.0*(float)V_ref;
    
    // cout << "average = " << average << endl;

    //cout << "vol_adc = " << vol << " mV" << endl;

    vol -= BiasVol;

    //cout << "vin_alc = " << vol/Av_Amplifer << " mV" << endl;

    float temp = K_VtoT(vol/Av_Amplifer) + __temp_n;
    
    
    
    // cout << "__temp_n = " << get_nt() << endl;
    // float temp = __temp_n + K_VtoT(vol/Av_Amplifer);

    return temp;
}

// push data, 1ms per time
int PVFM_Temp::pushDta()
{
    total= total - readings[index];
    // read from the sensor:
    readings[index] = getAnalog(__pin_k);
    // cout << "getAnalog = " << readings[index] << endl;

    total = total + readings[index];
    
    index++;
    index = index >= numReadings ? 0 : index;
    average = (total >> numReadings_2);

    return average;
}

float PVFM_Temp::get_nt()                                               // get temperature
{
    int a = getAnalog(__pin_n)*50/33;

    float resistance=(float)(1023-a)*10000/a;                           // get the resistance of the sensor;
    float temperature=1/(log(resistance/10000)/3975+1/298.15)-273.15;   // convert to temperature via datasheet ;

    return temperature;
}

PVFM_Temp ptp;