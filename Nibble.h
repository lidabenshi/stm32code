#ifndef __NIBBLE_H
#define __NIBBLE_H

void tempcode(void);//�����¶ȱ���
extern void tempcode_warm(void);//�����¶ȱ���

void timercode(void); //��ʱʱ�����
void setf0f1code(void);//f0f1��
void setF2code(void);
void setF3code(void) ;
void setF5code(void);


extern unsigned char DoubleNibble[4];
extern unsigned char DoubleNibbleH[5][5];
extern unsigned char CustomerCode[5];
extern unsigned char DoubleNibbleF5[4];
//extern unsigned char DoubleNibbleF5H[4];

#define nBIT0 0X01
#define nBIT1 0X02
#define nBIT2 0X04
#define nBIT3 0X08
#define nBIT4 0X10
#define nBIT5 0X20
#define nBIT6 0X40
#define nBIT7 0X80

extern void setnibble(void);
extern void tempcode_warm(void);//�����¶ȱ���
#endif