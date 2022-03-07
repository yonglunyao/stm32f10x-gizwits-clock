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
 ALIENTEK战舰V3\精英STM32开发板
 机智云-IOT_NTP网络时间获取实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


/* 用户区当前设备状态结构体*/
dataPoint_t currentDataPoint;
extern gizwitsProtocol_t gizwitsProtocol;


//wifi连接状态
u8 wifi_sta=0;//1:已连接 0:断开

int decimal_bcd_code(int decimal);

//协议初始化
void Gizwits_Init(void)
{
	TIM3_Int_Init(9,7199);//1MS系统定时
  usart3_init(9600);//WIFI初始化
	memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));//设备状态结构体初始化
	gizwitsInit();//缓冲区初始化
}
//数据采集
void userHandle(void)
{
    static u8 t=0;
	
	if(wifi_sta)
	{
		t++;
		if(t==10)
		{
			t=0;
			gizwitsGetNTP();//请求NTP网络时间
			
		}
		//delay_ms(100);
	}
	else
	{
		if(t!=0) t=0;
	}
	
}
//主函数
 int main(void)
 {		
    int key;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	    //串口初始化为115200
 	LED_Init();			    //LED端口初始化
	KEY_Init();             //按键初始化
  Seg_Init();	 
	esp8266_init();
  Gizwits_Init();         //协议初始化
	mcuRestart();//重启wifi模组
	printf("--------机智云IOT-NTP网络时间获取实验----------\r\n");
	printf("KEY1:AirLink连接模式\t KEY_UP:复位\r\n\r\n");
	 
   	while(1)
	{
	   userHandle();//用户采集
         
		 gizwitsHandle((dataPoint_t *)&currentDataPoint);//协议处理
	
		 key = KEY_Scan(0);
		if(key==KEY1_PRES)//KEY1按键
		{
			printf("WIFI进入AirLink连接模式\r\n");
			gizwitsSetMode(WIFI_SOFTAP_MODE);//Air-link模式接入
			
			Seg_DispNum(0x0E,0,0,0x01,0,0);
			
			int count = 100;
			while(count>0){
				delay_ms(100);
				LED0 = !LED0;
				count--;
			}
			
		}			
		if(key==WKUP_PRES)//KEY_UP按键
		{  
			printf("WIFI复位，请重新配置连接\r\n");
			gizwitsSetMode(WIFI_RESET_MODE);//WIFI复位
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
