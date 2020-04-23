#include "reg52.h"

//ʱ���������
#define WASHINE_TIME 30*60 //ϴ��ʱ��
#define RELEASE_TIME 2*60 //��ˮʱ��
#define DRY_TIME     5*60 //˦��ʱ��
#define STOP_TIME    45 //�ȴ�ɲ��ʱ��

//#define WASHINE_TIME 30 //ϴ��ʱ��
//#define RELEASE_TIME 10 //��ˮʱ��
//#define DRY_TIME     60 //˦��ʱ��
//#define STOP_TIME    35 //�ȴ�ɲ��ʱ��

//�̵������Ŷ���
sbit rotate1      = P1^3;
sbit rotate2      = P1^2;
sbit release      = P1^1;

//���������Ŷ���
sbit beep         = P1^5;

//ʱ��״̬�����������ж�ʱ����ϴ��ʱ�䣬��ˮʱ�䣬��ˮʱ�䣬�Ӷ����Ƶ��
//unsigned int timeState = WASHINE_TIME + RELEASE_TIME + DRY_TIME + STOP_TIME;
unsigned int timeState=0;
unsigned char rotateChagneTime = 0;// �����תʱ��
unsigned char Time0time = 0;  //��ʱ��0 ��ʱʱ��
unsigned char dryReadTime = 0; //˦������ʱ��
//ȫ������ʱ���������־
unsigned char beeFlag = 0;

//��������
sbit startStopKey = P3^7;
sbit releaseKey   = P3^5;

//��ʱ����
void delay(unsigned int i)
{
	while(i--);
}
//��ʱ����ʼ��
void InitTimer0(void)//��ʱ����ʼ�� ��ʱ 50,000ms
{
    TMOD = 0x01;
    TH0  = 0x4C;
    TL0  = 0x00;
    EA   = 1;
    ET0  = 1;
    TR0  = 1;
}
//����������ֹͣϴ��
void washineStartStopSetting(void)
{
	static char flag = 1;
	if(flag)
	{
		//����ʱ�������ʹ�俪ʼϴ��
		timeState = WASHINE_TIME + RELEASE_TIME + DRY_TIME + STOP_TIME;
		
		//��������
		beep = 0;
		delay(65535);delay(65535);delay(65535);
		beep = 1;
	}
	else
	{
		//����ʱ�������ʹ�俪ʼֹͣϴ��
		timeState = 0;
		
		//��������
		beep = 0;
		delay(65535);delay(65535);delay(65535);
		beep = 1;
		delay(65535);delay(65535);delay(65535);
		beep = 0;
		delay(65535);delay(65535);delay(65535);
		beep = 1;
	}
	flag=!flag;
}

//����������ֹͣ��ˮ/��ˮ
void releaseStartStopSetting(void)
{
	static char flag = 1;
	if(flag)
	{
		//����ʱ�������ʹ�俪ʼ��ˮ����ˮ
		timeState =  RELEASE_TIME + DRY_TIME + STOP_TIME;
		
		//��������
		beep = 0;
		delay(65535);delay(65535);delay(65535);
		beep = 1;
	}
	else
	{
		if(timeState>(DRY_TIME + STOP_TIME))//������ڷ�ˮ��ֱ��ֹͣ
		{
			//����ʱ�������ʹ��ֹͣ����
			timeState =  0;
		}
		else//�������˦�ɣ�ֱ�ӽ���ȴ�ɲ��
		{
			
			timeState =  STOP_TIME;//�ȴ�˦��ֹͣ
		}
		//��������
		beep = 0;
		delay(65535);delay(65535);delay(65535);
		beep = 1;
		delay(65535);delay(65535);delay(65535);
		//��������
		beep = 0;
		delay(65535);delay(65535);delay(65535);
		beep = 1;
	}
	
	flag=!flag;
}
void main(void)
{
	InitTimer0();
	beep = 0;
	delay(65535);delay(65535);delay(65535);
	delay(65535);delay(65535);delay(65535);
	delay(65535);delay(65535);delay(65535);
	beep = 1;
	while(1)
	{
		//����ֹͣ����
		if(startStopKey==0)
		{
			delay(5000);//��ʱ����
			if(startStopKey==0)
			{
				//���û������ˮ���ſ�ʼϴ�ӣ���������л�״̬������
				if(timeState==0 || timeState>(RELEASE_TIME + DRY_TIME + STOP_TIME))
				{
					washineStartStopSetting();//������ֹͣϴ��
					rotateChagneTime = 0;
					Time0time=0;
					
					rotate1 = 1;
					rotate2 = 1;
					release = 1;
					
				}
			}
			while(startStopKey==0);
		}
		
		//��ˮ/��ˮ��ť����
		if(releaseKey==0)
		{
			delay(5000);//��ʱ����
			if(releaseKey==0)
			{
				releaseStartStopSetting();//������ֹͣ��ˮ/��ˮ
				
				rotate1 = 1;
				rotate2 = 1;
				release = 1;
				
				
			}
			while(releaseKey==0);
		}
	}
}
void Timer0Interrupt(void) interrupt 1
{
	
	
	
    TH0 = 0x4C;//��ʱ 50,000ms
    TL0 = 0x00;
	
	Time0time++;
	if(Time0time>=20)//��ʱ1��//
	{
		Time0time=0;//
		
		rotateChagneTime++;
		
		if(timeState!=0)
		{
			timeState--;
		}
		
		dryReadTime++;
	}
	if(timeState > RELEASE_TIME + DRY_TIME + STOP_TIME)//ϴ��
	{
		if(rotateChagneTime==0){
			rotate1 = 1;
			rotate2 = 0;
		}
		else if(rotateChagneTime==5)
		{
			rotate1 = 1;
			rotate2 = 1;
		}
		else if(rotateChagneTime==8)
		{
			rotate1 = 0;
			rotate2 = 1;
		}
		else if(rotateChagneTime==13)
		{
			rotate1 = 1;
			rotate2 = 1;
		}
		else if(rotateChagneTime>=16)
		{
			rotateChagneTime=0;
		}
	}
	else if(timeState > DRY_TIME + STOP_TIME)//��ˮ
	{
			rotate1 = 1;
			rotate2 = 1;
			
			release = 0;
		
			dryReadTime=0 ;//��ˮʱ����0
	}
	else if(timeState > STOP_TIME)//��ˮ
	{
			if(dryReadTime==0)
			{	
				rotate1 = 0;
				rotate2 = 1;
				
				release = 0;
			}
			else if(dryReadTime==2)
			{	
				rotate1 = 1;
				rotate2 = 1;
				
				release = 0;
			}
			else if(dryReadTime==5)
			{	
				rotate1 = 0;
				rotate2 = 1;
				
				release = 0;
			}
			else if(dryReadTime==7)
			{	
				rotate1 = 1;
				rotate2 = 1;
				
				release = 0;
			}
			else if(dryReadTime==10)
			{	
				rotate1 = 0;
				rotate2 = 1;
				
				release = 0;
			}
			else if(dryReadTime==12)
			{	
				rotate1 = 1;
				rotate2 = 1;
				
				release = 0;
			}
			else if(dryReadTime==15)
			{	
				rotate1 = 0;
				rotate2 = 1;
				
				release = 0;
			}
			else if(dryReadTime==17)
			{	
				rotate1 = 1;
				rotate2 = 1;
				
				release = 0;
			}
			else if(dryReadTime==20)
			{	
				rotate1 = 0;
				rotate2 = 1;
				
				release = 0;
			}
			else if(dryReadTime==22)
			{	
				rotate1 = 1;
				rotate2 = 1;
				
				release = 0;
			}
			else if(dryReadTime==25)
			{	
				rotate1 = 0;
				rotate2 = 1;
				
				release = 0;
			}
			else if(dryReadTime==27)
			{	
				rotate1 = 1;
				rotate2 = 1;
				
				release = 0;
			}
			else if(dryReadTime>=30)
			{	
				dryReadTime=30;
				rotate1 = 0;
				rotate2 = 1;
				
				release = 0;
			}
		
	}
	else if(timeState > 0)//��ˮ���ȴ�˦��ֹͣ
	{
		rotate1 = 1;
		rotate2 = 1;
		
		release = 0;
		
		beeFlag = 1;//׼��ȫ������ʱ��������
	}
	else //���� ����
	{
		rotate1 = 1;
		rotate2 = 1;
		
		release = 1;
		
		//����ʱ ������ �ε� �� ��
		if(beeFlag==1)
		{
			beeFlag = 0;
			
			beep = 0;
			delay(65535);delay(65535);delay(65535);
			beep = 1;
			delay(65535);delay(65535);delay(65535);
			beep = 0;
			delay(65535);delay(65535);delay(65535);
			beep = 1;
			delay(65535);delay(65535);delay(65535);
			beep = 0;
			delay(65535);delay(65535);delay(65535);
			beep = 1;
			delay(65535);delay(65535);delay(65535);
			beep = 0;
			delay(65535);delay(65535);delay(65535);
			beep = 1;
			delay(65535);delay(65535);delay(65535);
			beep = 0;
			delay(65535);delay(65535);delay(65535);
			beep = 1;
			delay(65535);delay(65535);delay(65535);
			beep = 0;
			delay(65535);delay(65535);delay(65535);
			beep = 1;

		}
		
	}
}
