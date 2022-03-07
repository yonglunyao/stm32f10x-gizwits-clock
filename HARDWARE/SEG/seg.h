#ifndef _SEG_H_
#define _SEG_H_

#define GPUO_SEG							GPIOC
#define RCC_APB2Periph_SEG		RCC_APB2Periph_GPIOC
#define SEG_CLK_PIN						GPIO_Pin_8
#define SEG_DIO_PIN						GPIO_Pin_9
#define SEG_CS_PIN						GPIO_Pin_10
#define SEG_CLK 							PCout(8)
#define SEG_DIO 							PCout(9)
#define SEG_CS 			  				PCout(10)

//共阴数码管段码表

void Seg_Init(void);
void Seg_Start(void);
void Seg_Ask(void);
void Seg_Stop(void);
void Seg_WriteByte(unsigned char onebyte);
void Seg_DispNum(int num0,int num1,int num2,int num3,int num4,int num5);


#endif
