#include "esp8266.h"
#include "sys.h"
#include "delay.h"

void esp8266_init(void){
	 GPIO_InitTypeDef  GPIO_InitStructure;
 	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB.5 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
	 GPIO_SetBits(GPIOA,GPIO_Pin_8);						 //PB.5 �����
}

void esp8266_rst(void){
	ESP8266_RST = 0;
	delay_ms(10);
	ESP8266_RST = 1;

}
