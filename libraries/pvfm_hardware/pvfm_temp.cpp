

const float Var_VtoT_K[3][10]=
{
    {0, 2.5173462e1, -1.1662878, -1.0833638, -8.9773540/1e1, -3.7342377/1e1,
    -8.6632643/1e2, -1.0450598/1e2, -5.1920577/1e4},
    {0, 2.508355e1, 7.860106/1e2, -2.503131/1e1, 8.315270/1e2,
    -1.228034/1e2, 9.804036/1e4, -4.413030/1e5, 1.057734/1e6, -1.052755/1e8},
    {-1.318058e2, 4.830222e1, -1.646031, 5.464731/1e2, -9.650715/1e4,
    8.802193/1e6, -3.110810/1e8}
};


// Corresponding analog value of 100-500, per 10oC. such as xxx[0] means analogValue of 100, 
// xxx[10] means analogValue of 100+10*10 = 200oC

int temp_2_analog[];


PVFM_Temp(int pin_k, int pin_n)
{
    __pin_k = pin_k;
    __pin_n = pin_n;

    index       = 0;                        // the index of the current reading
    total       = 0;                        // the running total
    average     = 0;                        // the average
    average_buf = 0;
    
    temp_set(300);                          // default value: 300oC
    initDta();
}


float PVFM_Temp::K_VtoT(float mV)
{
    unsigned char i = 0;
    float value = 0;

#if 0
    if(mV >= -6.478 && mV < 0)
    {
        value = Var_VtoT_K[0][8];
        for(i = 8; i >0; i--)
        value = mV * value + Var_VtoT_K[0][i-1];
    }
    else if(mV >= 0 && mV < 20.644)
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
#else

    if(mV >= 0 && mV < 20.644)
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

#endif
    return (value + get_nt());
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

    cout << "vol_adc = " << vol << " mV" << endl;

    vol -= BiasVol;

    cout << "vin_alc = " << vol/Av_Amplifer << " mV" << endl;

    float temp = K_VtoT(vol/Av_Amplifer);

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

    index = index + 1;

    if (index >= numReadings)
    index = 0;

    average = (total >> numReadings_2);

    return average;
}

float PVFM_Temp::get_nt()                                   // get temperature
{

    int a = getAnalog(__pin_n)*50/33;

    // cout << "a = " << a << endl;

    float resistance=(float)(1023-a)*10000/a; //get the resistance of the sensor;
    float temperature=1/(log(resistance/10000)/3975+1/298.15)-273.15;//convert to temperature via datasheet ;

    return temperature;
}