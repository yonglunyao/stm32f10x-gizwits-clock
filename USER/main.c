#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "timer.h"
#include "seg.h"
#include "usart3.h"
#include "esp8266.h"
#include "gizwits_product.h" 
#include "gizwits_protocol.h" 
/************************************************
 ALIENTEKս��V3\��ӢSTM32������
 ������-IOT_NTP����ʱ���ȡʵ�� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


/* �û�����ǰ�豸״̬�ṹ��*/
dataPoint_t currentDataPoint;
extern gizwitsProtocol_t gizwitsProtocol;


//wifi����״̬
u8 wifi_sta=0;//1:������ 0:�Ͽ�

int decimal_bcd_code(int decimal);

//Э���ʼ��
void Gizwits_Init(void)
{
	TIM3_Int_Init(9,7199);//1MSϵͳ��ʱ
  usart3_init(9600);//WIFI��ʼ��
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//�豸״̬�ṹ���ʼ��
	gizwitsInit();//��������ʼ��
}
//���ݲɼ�
void userHandle(void)
{
    static u8 t=0;
	
	if(wifi_sta)
	{
		t++;
		if(t==10)
		{
			t=0;
			gizwitsGetNTP();//����NTP����ʱ��
			
		}
		//delay_ms(100);
	}
	else
	{
		if(t!=0) t=0;
	}
	
}
//������
 int main(void)
 {		
    int key;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	    //���ڳ�ʼ��Ϊ115200
 	LED_Init();			    //LED�˿ڳ�ʼ��
	KEY_Init();             //������ʼ��
  Seg_Init();	 
	esp8266_init();
  Gizwits_Init();         //Э���ʼ��
	mcuRestart();//����wifiģ��
	printf("--------������IOT-NTP����ʱ���ȡʵ��----------\r\n");
	printf("KEY1:AirLink����ģʽ\t KEY_UP:��λ\r\n\r\n");
	 
   	while(1)
	{
	   userHandle();//�û��ɼ�
         
		 gizwitsHandle((dataPoint_t *)&currentDataPoint);//Э�鴦��
	
		 key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1����
		{
			printf("WIFI����AirLink����ģʽ\r\n");
			gizwitsSetMode(WIFI_SOFTAP_MODE);//Air-linkģʽ����
			
			Seg_DispNum(0x0E,0,0,0x01,0,0);
			
			int count = 100;
			while(count>0){
				delay_ms(100);
				LED0 = !LED0;
				count--;
			}
			
		}			
		if(key==WKUP_PRES)//KEY_UP����
		{  
			printf("WIFI��λ����������������\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI��λ
			Seg_DispNum(0x0E,0,0,0x02,0,0);
			delay_ms(1000);		
		}
		
		if(wifi_sta==1){
			
				protocolTime_t *ptime = &(gizwitsProtocol.TimeNTP);
				u8 h = decimal_bcd_code(ptime->hour);
				u8 m = decimal_bcd_code(ptime->minute);
				u8 s = decimal_bcd_code(ptime->second);
							
				unsigned  char secLow=s&0x0F;
				unsigned  char secHigh=(s&0xF0)>>4;
				unsigned  char minLow=m&0x0F;
				unsigned  char minHigh=(m&0xF0)>>4;
				unsigned  char hourLow=h&0x0F;
				unsigned  char hourHigh=(h&0xF0)>>4;
	
				LED0 = 0;
				Seg_DispNum(hourHigh,hourLow,minHigh,minLow,secHigh,secLow);
		}
		else{
			LED0 = 1;	
			Seg_DispNum(0x0E,0,0,0x0F,0,0);
		
		}
		
		delay_ms(100);		
	}	 

} 
 
int decimal_bcd_code(int decimal)
{
	int sum = 0;  //sum???BCD?
 
	for (int i = 0; decimal > 0; i++)
	{
		sum |= ((decimal % 10 ) << ( 4*i));
 
		decimal /= 10;
	}
 
	return sum;
}
