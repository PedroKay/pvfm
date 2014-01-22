#include <Streaming.h>


#define SSR1temp	A0
#define SSR2temp	A1
#define SSR3temp 	A2

#define PINSSR1 	5
#define PINSSR2 	6
#define PINSSR3 	7

#define SSR1ON()       digitalWrite(PINSSR1, HIGH)
#define SSR1OFF()      digitalWrite(PINSSR1, LOW)
#define SSR2ON()       digitalWrite(PINSSR2, HIGH)
#define SSR2OFF()      digitalWrite(PINSSR2, LOW)
#define SSR3ON()       digitalWrite(PINSSR3, HIGH)
#define SSR3OFF()      digitalWrite(PINSSR3, LOW)


#define PINBTN      2                       // PIN OF BUTTON

const int numReadings = 32;
const int numReadings_2 = 5;                // pow(2, 6) = 64


const float temp_set = 400.0;


int readings[numReadings];                  // the readings from the analog input
int index       = 0;                        // the index of the current reading
long total      = 0;                        // the running total
int average     = 0;                        // the average
int average_buf = 0;


void initDta()
{
    for(int i=0; i<numReadings; i++)
    {
        readings[i] = 0;
    }
}


int pushDta(int dta)
{

    total= total - readings[index];         
    // read from the sensor:

    readings[index] = dta;

    total = total + readings[index];   

    index = index + 1;          

    if (index >= numReadings)              
    index = 0;                           
 
    average = (total >> numReadings_2);
    

    return average;
}

void initSSR()
{
    pinMode(PINSSR1, OUTPUT);
    pinMode(PINSSR2, OUTPUT);
    pinMode(PINSSR3, OUTPUT);
    
    SSR1OFF();
    SSR2OFF();
    SSR3OFF();
}

int getAnalog(int pin)
{

    long sum = 0;
    for(int i=0; i<8; i++)
    {
        sum+=analogRead(pin);
    }
    
    return sum>>3;
}


long timer1;

void setup()
{

    Serial.begin(115200);
    initSSR();
    
    pinMode(PINBTN, INPUT);
       
    
    initDta();
    
    
    cout << "hello world" << endl;  

    timer1 = millis();
}

int ssrstate = 0;


float getTemperature()
{
    // ssr off
    
    float vol = (float)average/1023.0*5000.0 - 25;
    float temp = K_VtoT(vol/268.0);
     
    if((temp_set-temp)<5)
    {
        int tmp = ssrstate;
        
        SSR1OFF();
        delay(5);
        
        for(int i=0; i<32; i++)
        {
            pushDta(getAnalog(SSR3temp));
        }

        vol = (float)average/1023.0*5000.0 - 25;
        temp = K_VtoT(vol/268.0);

        if(tmp)SSR1ON();
    }
    return temp;
    
}

void loop()
{

    pushDta(getAnalog(SSR3temp));

  
    if(millis()-timer1 > 100)
    {
        timer1 = millis();
        
        float temp_now = getTemperature();
        if(average != average_buf)
        {
            cout << (int)temp_now << endl;
            average_buf = average;
        }
        
        if(ssrstate == 0 && temp_now <= (temp_set-0.2))
        {
            SSR1ON();
            ssrstate = 1;
        }
        else if(ssrstate == 1 && temp_now >= (temp_set+0.5))
        {
            SSR1OFF();
            ssrstate = 0;
        }
        else if(temp_now > (temp_set+5))
        {
            SSR1OFF();
            ssrstate = 0;
        }
        else if(temp_now < (temp_set-5))
        {
            SSR1ON();
            ssrstate = 0;
        }
        
    }
    
    
    delay(1);
    
    if(digitalRead(PINBTN))
    {
        delay(20);
        if(digitalRead(PINBTN))
        {
            ssrstate = 1-ssrstate;
            
            if(ssrstate)
            SSR1ON();
            else
            SSR1OFF();
            
            while(digitalRead(PINBTN));
        }
    }

}


// K型电压转温度系数
float Var_VtoT_K[3][10]=
{
	{0, 2.5173462e1, -1.1662878, -1.0833638, -8.9773540/1e1, -3.7342377/1e1,
		-8.6632643/1e2, -1.0450598/1e2, -5.1920577/1e4},
	{0, 2.508355e1, 7.860106/1e2, -2.503131/1e1, 8.315270/1e2,
		-1.228034/1e2, 9.804036/1e4, -4.413030/1e5, 1.057734/1e6, -1.052755/1e8},
	{-1.318058e2, 4.830222e1, -1.646031, 5.464731/1e2, -9.650715/1e4,
		8.802193/1e6, -3.110810/1e8}
};

/******************************************************************************
*函数描述:K型 电压转温度(冷端温度为0)
		  毫伏范围：-5.891 ~ +54.819
		  温度范围：-199.933C° ~ +1370.057C°
*输入变量:mV 毫伏
*返回变量:value 温度
******************************************************************************/
float K_VtoT(float mV)
{
	unsigned char i = 0;
	float value = 0;
	
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
	
	return value;
}
