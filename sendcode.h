#ifndef _SendCode_H
#define _SendCode_H


//����͵�ƽ��ʱ��ֵ
#define Zero_time 7
#define One_time 21
#define head_time 59
#define empty_time 77 
//�ز���
#define carrier_head 340
#define carrier_data 24

void delay(unsigned int counter);
void carrier(unsigned int counter);

extern void delay_20ms(unsigned int counter);

extern void Send_Code(unsigned char codebyte);
extern void sendF5(void);

#endif
