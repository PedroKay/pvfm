#include <SerialLCD.h>
#include <SoftwareSerial.h> //this is a must
#include <MsTimer2.h>
#include <math.h>

#define PINSSR1 	5
#define PINSSR2 	6
#define PINSSR3 	7
#define buzzer 		8
#define buzzer_on 	1
#define buzzer_off	0

#define SSR1temp	A0
#define SSR2temp	A1
#define SSR3temp 	A2
#define SSRANG 		A3

#define SSR1ON       digitalWrite(PINSSR1, HIGH);
#define SSR1OFF      digitalWrite(PINSSR1, LOW);
#define SSR2ON       digitalWrite(PINSSR2, HIGH);
#define SSR2OFF      digitalWrite(PINSSR2, LOW);
#define SSR3ON       digitalWrite(PINSSR3, HIGH);
#define SSR3OFF      digitalWrite(PINSSR3, LOW);

int TEMP_CHAR[3];
float SSR1=0;
float SSR2=0;
float SSR3=0;

bool SSRERROR=0;
SerialLCD slcd(3,4);

void setup()
{

	pinMode(PINSSR1, OUTPUT);
	pinMode(PINSSR2, OUTPUT);
	pinMode(PINSSR3, OUTPUT);
	pinMode(buzzer, OUTPUT);
	pinMode(SSR1temp, INPUT);
	pinMode(SSR2temp, INPUT);
	pinMode(SSR3temp, INPUT);
	pinMode(SSRANG, INPUT);
	
	SSR1OFF;
	SSR2OFF;
	SSR3OFF;
	
	SSR_ERROR();
	//Timer1.initialize(100000); 
	MsTimer2::set(100, Att_ISR);
	//Timer1.attachInterrupt(Att_ISR);
	
	slcd.begin();
	slcd.backlight();
	slcd.clear();
	
	Serial.begin(115200);
	MsTimer2::set(100, Att_ISR);
	MsTimer2::start();  
	attachInterrupt(0, shutdown, RISING);
	/*
	digitalWrite(8,HIGH);
	Alarm(buzzer_on);
	delay(1000);
	Alarm(buzzer_off);*/
}



void loop()
{
	SSR_ERROR();
	TEMP_CTR();
}


void SSR_ALL_ON()
{
	delay(100);
	SSR1ON;
	delay(10);
	SSR1ON;
	delay(100);
	SSR2ON;
	delay(10);
	SSR2ON;
	delay(100);
	SSR3ON;
	delay(10);
	SSR3ON;
}

void SSR_ALL_OFF()
{
	SSR1OFF;
	SSR3OFF;
	SSR2OFF;
	delay(5);
	SSR1OFF;
	delay(5);
	SSR2OFF;
	delay(5);
	SSR3OFF;
}

void SSR_LOOP_ON(unsigned int SSRON_NUM)
{
if (SSRON_NUM==1)
{
	SSR1ON;
	delay(3000);
	SSR1OFF;
	delay(500);
	SSR2ON;
	delay(3000);
	SSR2OFF;
	delay(500);
	SSR3ON;
	delay(3000);
	SSR3OFF;
}
else if (SSRON_NUM==2)
{
	SSR_ALL_OFF();
	//num1 & 2 is on 
	SSR3OFF;
	SSR1ON;
	SSR2ON;
	delay(2000);
	SSR1OFF;
	
	//num2 & 3 is on
	//SSR2ON;
	SSR3ON;
	delay(2000);
	SSR2OFF;
	
	//num1 & 3 is on
	SSR1ON;
	//SSR3ON;
	delay(2000);
	//SSR2OFF;
}
else
{
	SSR_ALL_OFF();
	delay(100);
	SSR_ALL_OFF();
}

}

int ANG_TEMP_MAP()
{
float val=0;
  for(unsigned char i=0;i<20;i++)
  	{
	 val+= analogRead(SSRANG);
	 delay(5);
  	}
  val=val/20;
  val = map(val, 0, 1023, 0, 450);
  slcd.clear();
  slcd.setCursor(0,0);
  slcd.print("Set:");
  slcd.setCursor(4,0);
  slcd.print(val,DEC);
  return val;
  
}


int TEMP_MAP(int temp)//temp=SSR1temp;SSR2temp;SSR3temp
{
unsigned int val=0;
if (temp==SSR1temp)
{
	val=TEMP_CHAR[0];
}
else if (temp==SSR2temp)
{
	val=TEMP_CHAR[1];
}
  else if (temp==SSR3temp)
  {
	  val=TEMP_CHAR[2];
  }
  val = map(val, 0, 888, 0, 450);
  return val;

}

void Temp_Read()
{
TEMP_CHAR[0]=0;
TEMP_CHAR[1]=0;
TEMP_CHAR[2]=0;
for(unsigned char i=0;i<20;i++)
{
	TEMP_CHAR[0]+=analogRead(SSR1temp);
	TEMP_CHAR[1]+=analogRead(SSR2temp);
	TEMP_CHAR[2]+=analogRead(SSR3temp);
	delay(10);
}
TEMP_CHAR[0]=TEMP_CHAR[0]/20;
TEMP_CHAR[1]=TEMP_CHAR[1]/20;
TEMP_CHAR[2]=TEMP_CHAR[2]/20;

}

float VOL_CONVERT(unsigned int voltage)
{
voltage=voltage*5/1023.0/100.0;
return voltage;

}

void TEMP_CTR()
{
	Alarm(buzzer_off);
    
if (SSRERROR==0)
{
	Temp_Read();
	int preset_temp = ANG_TEMP_MAP();
	
	SSR1=TEMP_MAP(SSR1temp);
	SSR2=TEMP_MAP(SSR2temp);
	SSR3=TEMP_MAP(SSR3temp);
	
	Serial.print("SSR1 ");
	Serial.println(SSR1);
	
	Serial.print("SSR2 ");
	Serial.println(SSR2);
	
	Serial.print("SSR3 ");
	Serial.println(SSR3);
		
	if((preset_temp-SSR1>200)&&(preset_temp-SSR2>200)&&(preset_temp-SSR3>200))
	{
	
	Serial.println("SSR_ALL_ON");
	
	//slcd.setCursor(15,0);
	//slcd.print("3");
	
	SSR_ALL_ON();
	delay(2000);
	}
	
	else if((preset_temp-SSR1>100)&&(preset_temp-SSR2>100)&&(preset_temp-SSR3>100))
	{

	Serial.println("SSR_LOOP_ON2");
	
	//slcd.setCursor(15,0);
	//slcd.print("2");
	
	SSR_LOOP_ON(2);
	delay(3000);
	}
	else if((preset_temp-SSR1>50)&&(preset_temp-SSR2>50)&&(preset_temp-SSR3>50))
	{

	Serial.println("SSR_LOOP_ON1");
	
	//slcd.setCursor(15,0);
	//slcd.print("1");
	
	SSR_LOOP_ON(1);
	delay(5000);
	}
	else if ((SSR1-preset_temp>30)&&(SSR2-preset_temp>30)&&(SSR3-preset_temp>30))
	{
	SSR_ALL_OFF();

}
else
	SSR_ALL_OFF();

}
	
}
void Alarm(unsigned int statue)
{
	digitalWrite(buzzer,statue);
	//delay(1000);
}

int SSR_ERROR()
{
	unsigned int 	temp_max=0;
	unsigned char 	num=0;
	int 	SSR_temp[3];
	
	Temp_Read();
	
	SSR_temp[0]=TEMP_MAP(SSR1temp);
	SSR_temp[1]=TEMP_MAP(SSR2temp);
	SSR_temp[2]=TEMP_MAP(SSR3temp);

	Serial.print("SSR_temp[0] ");
	Serial.println(SSR_temp[0]);
	Serial.print("SSR_temp[1] ");
	Serial.println(SSR_temp[1]);
	Serial.print("SSR_temp[2] ");
	Serial.println(SSR_temp[2]);


	for(unsigned char i=0;i<3;i++)
		{
		if (SSR_temp[i]>temp_max)
			{
			temp_max=SSR_temp[i];
			num=i;
			}
		}
	
	Serial.print("SSR_temp ");
	Serial.println(temp_max);
	Serial.print("num ");
	Serial.println(num);
	
	if (num==0)
		{
			if ((((SSR_temp[1]+SSR_temp[2])/2)-SSR_temp[0]>100)||(SSR_temp[0]-((SSR_temp[1]+SSR_temp[2])/2)>100))
			{
			Serial.println(((SSR_temp[1]+SSR_temp[2])/2)-SSR_temp[0]);
			SSR_ALL_OFF();
			Serial.println("SSR1 ERROR!");
			SSRERROR=1;
			Alarm(buzzer_on);
			}
			else
				SSRERROR=0;
		}
	else if (num==1)
		{
			if ((((SSR_temp[1]+SSR_temp[0])/2)-SSR_temp[1]>100)||(SSR_temp[1]-((SSR_temp[1]+SSR_temp[0])/2)>100))
			{
			Serial.println(((SSR_temp[1]+SSR_temp[0])/2)-SSR_temp[1]);
			Serial.println(SSR_temp[1]-((SSR_temp[1]+SSR_temp[0])/2));
			SSR_ALL_OFF();
			Serial.println("SSR2 ERROR!");
			SSRERROR=1;
			Alarm(buzzer_on);
			}
			else
				SSRERROR=0;
		}
	else if (num==2)
		{
			if ((((SSR_temp[1]+SSR_temp[0])/2)-SSR_temp[2]>100)||(SSR_temp[2]-((SSR_temp[1]+SSR_temp[0])/2)>100))
			{
			Serial.println(((SSR_temp[1]+SSR_temp[0])/2)-SSR_temp[2]);
			SSR_ALL_OFF();
			Serial.println("SSR3 ERROR!");
			SSRERROR=1;
			Alarm(buzzer_on);
			}
			else
				SSRERROR=0;
		}

}

void Att_ISR()
{
	
	slcd.clear();
	ANG_TEMP_MAP();
	//slcd.clear();
	slcd.setCursor(0,1);
	slcd.print("1:");
	slcd.print(SSR1,DEC);
	slcd.setCursor(5,1);
	slcd.print("2:");
	slcd.print(SSR2,DEC);
	slcd.setCursor(10,1);
	slcd.print("3:");
	slcd.print(SSR3,DEC);
}

void shutdown()
{
	while(1)
	{
		SSR1OFF;
		SSR2OFF;
		SSR3OFF;
		digitalWrite(8,HIGH);
	}
}


/******************************************************************************
*函数描述:K型 电压转温度(冷端温度为0)
		  毫伏范围：-5.891 ~ +54.819
		  温度范围：-199.933C° ~ +1370.057C°
*输入变量:mV 毫伏
*返回变量:value 温度
******************************************************************************/
float K_VtoT(float mV)
{
	uchar i = 0;
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

