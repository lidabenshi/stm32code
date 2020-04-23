#ifndef _SendCode_H
#define _SendCode_H


//定义低电平延时的值
#define Zero_time 7
#define One_time 21
#define head_time 59
#define empty_time 77 
//载波数
#define carrier_head 340
#define carrier_data 24

void delay(unsigned int counter);
void carrier(unsigned int counter);

extern void delay_20ms(unsigned int counter);

extern void Send_Code(unsigned char codebyte);
extern void sendF5(void);

#endif
