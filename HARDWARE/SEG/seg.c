#include "seg.h"
#include "sys.h"
#include "delay.h"

int CodeTab[]={
		0x3f,0x06,0x5b,0x4f,
		0x66,0x6d,0x7d,0x07,
		0x7f,0x6f,0x77,0x7c,
		0x39,0x5e,0x79,0x71
};

void Seg_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SEG, ENABLE);//使能GPIOB时钟
 
  //GPIOFB3,4,5初始化设置
  GPIO_InitStructure.GPIO_Pin = SEG_CLK_PIN|SEG_DIO_PIN|SEG_CS_PIN;//PB3~5复用功能输出	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//通用输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_Init(GPUO_SEG, &GPIO_InitStructure);//初始化
}

void Seg_Start(void)
{
	SEG_CLK=1;
	SEG_DIO=1;
	delay_us(10);
	SEG_DIO=0;
}

void Seg_Ask(void)
{
	SEG_CLK=0;
	delay_us(10);
	SEG_CLK=1;
	delay_us(10);
	SEG_CLK=0;
}

void Seg_Stop(void)
{
	SEG_CLK=0;
	delay_us(10);
	SEG_DIO=0;
	delay_us(10);
	SEG_CLK=1;
	delay_us(10);
	SEG_DIO=1;
}

//写一个字节
void Seg_WriteByte(unsigned char onebyte)
{
	int i;
	for(i=0;i<8;i++)
	{
		SEG_CLK=0;
		if(onebyte&0x01)
		{
			SEG_DIO=1;
		}
		else
		{
			SEG_DIO=0;
		}
		delay_us(10);
		onebyte=onebyte>>1;
		SEG_CLK=1;
		delay_us(10);
	}
}

//显示数字
void Seg_DispNum(int num0,int num1,int num2,int num3,int num4,int num5)//秒不显示，用于闪烁dp
{
	if(num0<0||num0>15||num1<0||num1>15||num2<0||num2>15||num3<0||num3>15)
	{
		return;
	}
	Seg_Start();
	Seg_WriteByte(0x40);	//40H地址自动加1
	Seg_Ask();
	Seg_Stop();
	Seg_Start();
	Seg_WriteByte(0xC0);	//设置起始地址
	Seg_Ask();
	
	if(num5%2==0)
	{
		CodeTab[num0]=CodeTab[num0]|0x80;
	}
	else
	{
		CodeTab[num0]=CodeTab[num0]&0x7f;
	}
	Seg_WriteByte(CodeTab[num0]);
	Seg_Ask();
	
	if(num5%2==0)
	{
		CodeTab[num1]=CodeTab[num1]|0x80;
	}
	else
	{
		CodeTab[num1]=CodeTab[num1]&0x7f;
	}
	Seg_WriteByte(CodeTab[num1]);
	Seg_Ask();
	
	if(num5%2==0)
	{
		CodeTab[num2]=CodeTab[num2]|0x80;
	}
	else
	{
		CodeTab[num2]=CodeTab[num2]&0x7f;
	}
	Seg_WriteByte(CodeTab[num2]);
	Seg_Ask();
	
	if(num5%2==0)
	{
		CodeTab[num3]=CodeTab[num3]|0x80;
	}
	else
	{
		CodeTab[num3]=CodeTab[num3]&0x7f;
	}
	Seg_WriteByte(CodeTab[num3]);
	Seg_Ask();
	
	Seg_Stop();
	Seg_Start();
	Seg_WriteByte(0x8f);//亮度最大
	Seg_Ask();
	Seg_Stop();
}
