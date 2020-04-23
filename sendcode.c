#include "sendcode.h"
#include "Nibble.h"
#include "config.h"

// unsigned char DoubleNibble[4]={0x00,0x09,0x20,0x00};
// unsigned char CustomerCode[2]={0xa5,0xa6};
// unsigned char DoubleNibble_Final[2][4]={0x00,0x20,0,0};

/*延时函数 低电平延时时间*/
void delay(unsigned int counter) { // counter-15 549us -16 560ns -30 1ms -48
                                   // 1.65ms -129 -4.5ms
                                   // counter=15+(time-550)/35
  unsigned char i, j;
  for (i = 0; i < counter; i++) {
    for (j = 0; j < 50; j++)
      ;
  }
}
/*延时函数 低电平延时时间*/
void delay_20ms(unsigned int counter) { // counter 170 20ms
  unsigned int i, j;
  for (i = 0; i < counter; i++) {

    for (j = 0; j < 50; j++)
      ;
  }
}
/*载波函数 通过改变counter的值来改变载波发生时间
  //counter 340 time 9ms   24 0.624ms
*/ // 4M总线频率
void carrier(unsigned int counter) {
  unsigned int i, j;
  for (j = 0; j < counter; j++) {
    PTCD_PTCD3 = 1;
    for (i = 0; i < 1; i++) {
      asm nop;
      asm nop;
      // asm nop;
    }
    PTCD_PTCD3 = 0;
    for (i = 0; i < 1; i++) { //每增加1 时间计算方式为 4.25+（n-1）*2.6us
      i++;
      asm nop;
      // i++;
      asm nop;
      asm nop;
    }
  }
}

/*发送函数

*/
void Send_Code(unsigned char codebyte) {

  unsigned char bitcounter;
  unsigned char bytecounter;
  unsigned char temp;
  unsigned char Nibbletab[4];
  temp = 0;
  bitcounter = 0;
  bytecounter = 0;

  if (4 == codebyte) // F5码 转成4为了方便数组
  {

    if (keepwarmflag == 1) {

      DoubleNibbleF5[1] |= nBIT2;
      tempcode_warm();
    }
    DoubleNibbleF5[1] |= enjoywarmmode;
    if ((enjoywarmflag != 1) && (keepwarmflag != 1)) {

      DoubleNibbleF5[1] |= (fandirtemp[mode_temp] << 4);
    } else {

      DoubleNibbleF5[1] |= (0x03 << 4);
    }

    for (bytecounter = 0; bytecounter < 4; bytecounter++) {
      Nibbletab[bytecounter] = DoubleNibbleF5[bytecounter];
    }
  } else {
    for (bytecounter = 0; bytecounter < 4; bytecounter++) {

      Nibbletab[bytecounter] = DoubleNibble[bytecounter];
    }
  }

  DoubleNibbleH[codebyte][3] &= 0x0f;

  temp = ((Nibbletab[0] & 0x0f) + (Nibbletab[1] & 0x0f) +
          (Nibbletab[2] & 0x0f) + (Nibbletab[3] & 0x0f));
  temp += (CustomerCode[codebyte] & 0xf0) >> 4;
  temp +=
      ((DoubleNibbleH[codebyte][0] >> 4) + (DoubleNibbleH[codebyte][1] >> 4) +
       (DoubleNibbleH[codebyte][2] >> 4) + (DoubleNibbleH[codebyte][3] >> 4));
  DoubleNibbleH[codebyte][3] |= ((temp << 4) & 0xf0);

  //是否把中断关了？？？?
  asm SEI; //屏蔽中断服务程序?

  carrier(carrier_head);
  PTCD_PTCD3 = 0;
  delay(head_time);

  for (bytecounter = 0; bytecounter < 3; bytecounter++) { // nibble 1~6

    bitcounter = 1;
    while (bitcounter != 0) {

      carrier(carrier_data);
      PTCD_PTCD3 = 0;

      if (Nibbletab[bytecounter] & bitcounter) {

        delay(One_time);
      }

      else {

        delay(Zero_time);
      }

      bitcounter = (bitcounter << 1);
    }
  }

  if (bytecounter == 3) { // nibble7

    bitcounter = 1;

    while (bitcounter <= 0x08) {

      carrier(carrier_data);
      PTCD_PTCD3 = 0;

      if ((Nibbletab[bytecounter]) & bitcounter) {

        delay(One_time);
      }

      else {

        delay(Zero_time);
      }

      bitcounter = (bitcounter << 1);
    }
  }

  //发送客户码
  bitcounter = 1;
  while (bitcounter <= 0x40) { // send CustomerCode

    carrier(carrier_data);
    PTCD_PTCD3 = 0;
    if ((CustomerCode[codebyte]) & bitcounter) {

      delay(One_time);
    } else {
      delay(Zero_time);
    }

    bitcounter = (bitcounter << 1);
  }
  //发送0延时20ms
  carrier(carrier_data);
  PTCD_PTCD3 = 0;
  delay_20ms(empty_time);

  //
  for (bytecounter = 0; bytecounter < 4; bytecounter++) { // nibble 10~17

    bitcounter = 1;
    while (bitcounter != 0) {

      carrier(carrier_data);
      PTCD_PTCD3 = 0;
      if ((DoubleNibbleH[codebyte][bytecounter]) & bitcounter) {
        delay(One_time);
      } else {
        delay(Zero_time);
      }

      bitcounter = (bitcounter << 1);
    }
  }
  //结束码
  carrier(carrier_data);
  PTCD_PTCD3 = 0;

  asm CLI; //允许执行中断服务程序
}
