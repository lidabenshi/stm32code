#include "display.h"
#include "config.h"
#include "keyfunction.h"

const unsigned char timerTAB_eight[] = //八字表DEFA XCGB
    {
        0b11110101, // 0
        0b00000101, // 1
        0b11010011, // 2
        0b10010111, // 3
        0b00100111, // 4
        0b10110110, // 5
        0b11110110, // 6
        0b00010101, // 7
        0b11110111, // 8
        0b10110111  // 9
};

const unsigned char tempTAB_eight[] = // FGEX ABCD
    {

        0b10101111, // 0
        0b00000110, // 1
        0b01101101, // 2
        0b01001111, // 3
        0b11000110, // 4
        0b11001011, // 5
        0b11101011, // 6
        0b00001110, // 7
        0b11101111, // 8
        0b11001111, // 9
        0b11100110, // H
        0b11101110, // A
        0b10101001, // c
        0b01101111, // d
        0b11101001, // e
        0b11001000  // f

};

void dispclear(void) {

  remotY0.Byte = 0;
  remotY1.Byte = 0;
  remotY2.Byte = 0;
  remotY3.Byte = 0;
  remotY4.Byte = 0;
  remotY5.Byte = 0;
  remotY6.Byte = 0;
  remotY7.Byte = 0;
  remotY8.Byte = 0;
  remotY9.Byte = 0;
  remotY10.Byte = 0;
  remotY11.Byte = 0;
  remotY12.Byte = 0;
  remotY13.Byte = 0;
  remotY14.Byte = 0;
  remotY15.Byte = 0;
  remotY16.Byte &= (lockflag << 3);
  remotY17.Byte = 0;
  remotY18.Byte = 0;
  remotY19.Byte = 0;
  remotY20.Byte = 0;
  remotY21.Byte = 0;
  remotY22.Byte = 0;
  remotY23.Byte = 0;
  remotY24.Byte = 0;
  remotY25.Byte = 0;
  remotY26.Byte = 0;
  remotY27.Byte = 0;
  remotY28.Byte = 0;
  remotY29.Byte = 0;
  remotY30.Byte = 0;
  remotY31.Byte = 0;
}

unsigned char checktimeronset(void) {
  if ((onoffflag == 0) && ((timersta == 0) || (timersta >= 12))) {
    return 1;
  } else {
    return 0;
  }
}

void dispsleephour(void) {
  disp_timerLl = timerTAB_eight[sleepturbocounter & 0x0f];
  disp_timerLh = timerTAB_eight[sleepturbocounter & 0x0f] >> 4;
  disp_timerlater = 1;
}

void disptempnum(void) {

  if (HScounter >= 1 && HScounter < H1Twinklecounter) // H1 模式
  {
    if (H1twinkleflag == 1) {
      digittemp = 0xa1;
      disp_tempLl = tempTAB_eight[digittemp & 0x0f];
      disp_tempLh = tempTAB_eight[digittemp & 0x0f] >> 4;

      disp_tempHl = tempTAB_eight[(digittemp & 0xf0) >> 4];
      disp_tempHh = tempTAB_eight[(digittemp & 0xf0) >> 4] >> 4;

      disp_tempunitC = 0;
      disp_tempunitF = 0;
    }
    return;
  }
  //睡眠曲线显示
  if (sleepturbocounter != 0) {

    if (charflash) {
      if (tempuintflag == 0) {
        if (mode_temp == 0x01) {
          digittemp = cold_sleeptemps[sleepturbocounter];
        } else {

          digittemp = hot_sleeptemps[sleepturbocounter];
        }
        disp_tempunitC = 1;
      } else {
        if (mode_temp == 0x01) {
          digittemp = cold_sleeptemph[sleepturbocounter];
        } else {
          digittemp = hot_sleeptemph[sleepturbocounter];
        }

        disp_tempunitF = 1;
      }

      disp_tempLl = tempTAB_eight[digittemp & 0x0f];
      disp_tempLh = tempTAB_eight[digittemp & 0x0f] >> 4;

      disp_tempHl = tempTAB_eight[(digittemp & 0xf0) >> 4];
      disp_tempHh = tempTAB_eight[(digittemp & 0xf0) >> 4] >> 4;
    }
    return;
  }

  if (((onoffflag == 1) && (mode_temp == 0x00)) ||
      ((mode_temp == 0x00) && (!checktimeronset()))) { //自动模式下没有显示
    return;
  }

  /*暖享模式下 显示AH*/
  if (enjoywarmflag == 1) {

    digittemp = 0xba;
    disp_tempLl = tempTAB_eight[digittemp & 0x0f];
    disp_tempLh = tempTAB_eight[digittemp & 0x0f] >> 4;

    disp_tempHl = tempTAB_eight[(digittemp & 0xf0) >> 4];
    disp_tempHh = tempTAB_eight[(digittemp & 0xf0) >> 4] >> 4;

    return;
  }

  if ((saveflag == 1) && ((onoffflag == 1) || (!checktimeronset()))) {
    digittemp = 0x5e; // SE

    disp_tempLl = tempTAB_eight[digittemp & 0x0f];
    disp_tempLh = tempTAB_eight[digittemp & 0x0f] >> 4;

    disp_tempHl = tempTAB_eight[(digittemp & 0xf0) >> 4];
    disp_tempHh = tempTAB_eight[(digittemp & 0xf0) >> 4] >> 4;

    disp_tempunitC = 0;
    disp_tempunitF = 0;
    // disp_save=1;
    return;
  }

  /*保暖*/
  if (keepwarmflag == 1) {
    if (tempuintflag == 1) 
    {
      digittemp = keepwarmtemph;
    } 
    else 
    {
      digittemp = keepwarmtemps;
    }

  } 
  else {

    if (tempuintflag == 1) 
    {
      digittemp = tempvalh[mode_temp];
       
      
    } 
    else 
    {
      digittemp = tempvals[mode_temp];
      
    }
  }
  disp_tempLl = tempTAB_eight[digittemp & 0x0f];
  disp_tempLh = tempTAB_eight[digittemp & 0x0f] >> 4;

  disp_tempHl = tempTAB_eight[(digittemp & 0xf0) >> 4];
  disp_tempHh = tempTAB_eight[(digittemp & 0xf0) >> 4] >> 4;

  if (tempuintflag == 1) {

    disp_tempunitF = 1;
  } else {

    disp_tempunitC = 1;
  }
}

//定时开关显示
void disptimer(void) {
  if (sleepturbocounter != 0) {

    disp_timerLl = timerTAB_eight[sleepturbocounter & 0x0f];
    disp_timerLh = timerTAB_eight[sleepturbocounter & 0x0f] >> 4;
    disp_timerlater = 1;

    return;
  }

  if (timersta != 0) {
    digittimer = sethour;
    disp_timerLl = timerTAB_eight[digittimer & 0x0f];
    disp_timerLh = timerTAB_eight[digittimer & 0x0f] >> 4;

    digittimer &= 0xf0;
    if (digittimer == 0x20) {
      disp_timerHl = timerTAB_eight[2];
      disp_timerHh = timerTAB_eight[2] >> 4;
    }
    if (digittimer == 0x10) {
      disp_timerHl = timerTAB_eight[1];
      disp_timerHh = timerTAB_eight[1] >> 4;
    }

    if (sethalfhour != 0) {
      disp_timerfivel = timerTAB_eight[5];
      disp_timerfiveh = timerTAB_eight[5] >> 4;
      disp_timerpoint = 1;
    }

    if (timersta < 12 && (charflash == 0)) {
      if (onoffflag) {
        disp_timerlater = 1;
        disp_timeroff = 1;
      } else {
        disp_timerlater = 1;
        disp_timeron = 1;
      }
    } else if (timersta == 12) {
      if (onoffflag) {
        disp_timerlater = 1;
        disp_timeroff = 1;
      } else {
        disp_timerlater = 1;
        disp_timeron = 1;
      }
    }
  }
}

//灯光显示
void displight(void) {
  if (lightflag == 1) {

    disp_light = 1;
  }
}

/*健康显示
void disphealth(void)
{
        if(healthflag==1)
                disp_health=1;
}

//wifi显示
void dispwifi(void)
{
        if(wififlag==1)
                disp_wifi=1;
}
  */
//上下扫风闪烁
void dispswingudtwinkle(void) {
  if ((uddiscounter != 0) && (UDswingflash == 1)) {
    disp_swingudword = 1;
    disp_swingud1 = 1;
    disp_swingud2 = 1;
    disp_swingud3 = 1;
    disp_swingud4 = 1;
    disp_swingud5 = 1;
  }
}

//左右扫风闪烁
/*
void dispswinglrtwinkle(void)
{

         if((lrdiscounter!=0)&&(LRswingflash==1))
   {
      disp_swinglrword=1;
                disp_swinglr1=1;
                disp_swinglr2=1;
                disp_swinglr3=1;
                disp_swinglr4=1;
                disp_swinglr5=1;
   }
} */

//电池显示
void disbatteryvol(void) {
  switch (voltage) {
  case 3:
    disp_batteryshell = 1;
    disp_battery1 = 1;
    disp_battery2 = 1;
    disp_battery3 = 1;
    break;
  case 2:
    disp_batteryshell = 1;
    disp_battery1 = 1;
    disp_battery2 = 1;
    break;
  case 1:
    disp_batteryshell = 1;
    disp_battery1 = 1;
    break;
  case 0:
    disp_batteryshell = 1;
    break;
  default:
    break;
  }
}

//睡眠模式显示
void dispsleep(void) {
  if (sleepmode != 0) {
    disp_sleep = 1;

    switch (sleepmode) {
    case 1:
      disp_sleepmodeb = 1;
      disp_sleepmodec = 1;
      break;
    case 2:
      disp_sleepmodea = 1;
      disp_sleepmodeb = 1;
      disp_sleepmoded = 1;
      disp_sleepmodee = 1;
      disp_sleepmodeg = 1;
      break;
    case 3:
      disp_sleepmodea = 1;
      disp_sleepmodeb = 1;
      disp_sleepmodec = 1;
      disp_sleepmoded = 1;
      disp_sleepmodeg = 1;
      break;
    case 4:
      disp_sleepmodeb = 1;
      disp_sleepmodec = 1;
      disp_sleepmodef = 1;
      disp_sleepmodeg = 1;
      break;
    default:
      break;
    }
  }
}

//模式显示
void dispmode(void) {
  switch (mode_temp) {
  case 0:
    disp_modeauto = 1;
    break;
  case 1:
    disp_modecold = 1;
    break;
  case 2:
    disp_modewet = 1;
    break;
  case 3:
    disp_modewind = 1;
    break;
  case 4:
    disp_modehot = 1;
    break;
  default:
    break;
  }
}

//风速显示
void dispfansp(void) {
  unsigned char fansp;
  if (slientcount == 1) {

    fansp = 0;
  } else {
    fansp = fanspeedtemp[mode_temp];
  }
  disp_fanspword = 1;
  if (turoflag == 1) {
    disp_tubro = 1;
  }
  switch (fansp) {
  case 0:
    disp_fanspauto = 1;
    break;
  case 1:
    disp_fansp1 = 1;
    break;
  case 2:
    disp_fansp1 = 1;
    disp_fansp2 = 1;
    break;
  case 3:
    disp_fansp3 = 1;
    disp_fansp1 = 1;
    disp_fansp2 = 1;
    break;
  case 4:
    disp_fansp4 = 1;
    disp_fansp3 = 1;
    disp_fansp1 = 1;
    disp_fansp2 = 1;
    break;
  case 5:
    disp_fansp5 = 1;
    disp_fansp4 = 1;
    disp_fansp3 = 1;
    disp_fansp1 = 1;
    disp_fansp2 = 1;
    break;
  default:
    break;
  }
}

//风向选择
void dispfandir(void) {

  
  /*20170526 智暖模式下显示为上下出风
  if (enjoywarmmode || keepwarmflag) {

    disp_fandirupword = 0;
    disp_fandirdownword = 0;
    disp_fandirup = 0;
    disp_fandirdown = 0;
    disp_fandirmid = 0;
  } else   */
  if (!enjoywarmmode && !keepwarmflag && (0!=mode_temp))
  {
    disp_fandirmid = 1;
    switch (fandirtemp[mode_temp]) {

    case 1:
      disp_fandirup = 1;
      disp_fandirupword = 1;
      break;
    case 2:
      disp_fandirdown = 1;
      disp_fandirdownword = 1;
      break;
    case 3:
      disp_fandirupword = 1;
      disp_fandirdownword = 1;
      disp_fandirup = 1;
      disp_fandirdown = 1;
      break;
    default:
      disp_fandirautoword = 1;
      disp_fandirup = 1;
      disp_fandirdown = 1;
      break;
    }
  }
}

//上下扫风
void dispswingud(void) {

  if (swingudsta[mode_temp] != 0) //非扫风停显示上下扫风图标
  {
    disp_swingudword = 1;
  }
  switch (swingudsta[mode_temp]) {

  case 0: {
    break;
  }
  case 1: {
    disp_swingud1 = 1;
    disp_swingud2 = 1;
    disp_swingud3 = 1;
    disp_swingud4 = 1;
    disp_swingud5 = 1;
    break;
  }
  case 2: {
    disp_swingud1 = 1;
    break;
  }
  case 3: {
    disp_swingud2 = 1;
    break;
  }
  case 4: {
    disp_swingud3 = 1;
    break;
  }
  case 5: {
    disp_swingud4 = 1;
    break;
  }
  case 6: {
    disp_swingud5 = 1;
    break;
  }

  default:
    break;
  }
}
/*
//左右扫风显示
void dispswinglr(void)
{
       if (swinglrsta[mode_temp]!=0x00)   //非扫风停显示上下扫风图标
  {
                 disp_swinglrword=1;
  }

         switch(swinglrsta[mode_temp])
   {

      case 0:
      {
                  break;
      }
      case 1:
      {
               disp_swinglr1=1;
               disp_swinglr2=1;
               disp_swinglr3=1;
               disp_swinglr4=1;
               disp_swinglr5=1;
               break;
      }
      case 2:
      {
                 disp_swinglr1=1;
                  break;
      }
      case 3:
      {
           disp_swinglr2=1;
               break;
      }
      case 4:
      {
           disp_swinglr3=1;
                  break;
      }
      case 5:
      {
           disp_swinglr4=1;
                  break;
      }
      case 6:
      {
            disp_swinglr5=1;
                  break;
      }
     case 7:
     {
             disp_swinglr1=1;
            disp_swinglr5=1;
            break;
     }
     case 8: {

       if((swingcount==0x01)||(swingcount==0x02)) {

           disp_swinglr1=1;
           disp_swinglr5=1;
       }
       else
if((swingcount==0x03)||(swingcount==0x04)||(swingcount==0x07)||(swingcount==0x08))
       {
            disp_swinglr2=1;
           disp_swinglr4=1;
       }
       else if((swingcount==0x05)||(swingcount==0x06))
       {
           disp_swinglr3=1;
       }
     }
      break;
      default:
       break;
   }
}
*/
//静音显示
void dispslient(void) {
  if (slientcount == 0) {

    disp_slient = 0;
  } else {

    disp_slient = 1;
  }
  /*
  if(slientcount==1)
  {
          disp_fanspauto=1;
  }*/
}
/*强劲显示*/
void dispturbo(void) {

  if (turoflag == 1) {

    disp_tubro = 1;
  }
}

/*暖享显示*/

void dispenjoywarm(void) {

  switch (enjoywarmmode) {

  case 1:
    disp_enjoywarm1 = 1;
    break;
  case 2:
    disp_enjoywarm2 = 1;
    break;
  case 3:
    disp_enjoywarm3 = 1;
    break;
  default:
    disp_enjoywarm1 = 0;
    disp_enjoywarm2 = 0;
    disp_enjoywarm3 = 0;
    break;
  }
}

void dispkeepwarm(void) {

  if (1 == keepwarmflag) {

    disp_keepwarm = 1;
  }
}
/*
void displock() {

  if((lockflag==1)&&(locktwinklecount==0)) {

      disp_lock=1;
  }
} */
//临时变量赋值
void dispset(void) {
  LCDWF0 = remotY0.Byte; // 8
  LCDWF1 = remotY1.Byte; // 7
  LCDWF2 = remotY2.Byte; // 6
  LCDWF3 = remotY3.Byte; // 5
  LCDWF4 = remotY4.Byte; // 4
  LCDWF5 = remotY5.Byte; // 3
  LCDWF6 = remotY6.Byte; // 2
  LCDWF7 = remotY7.Byte; // 1
  // LCDWF8=remotY8.Byte;// 未使用
  // LCDWF9=remotY9.Byte;// 未使用
  // LCDWF10=remotY10.Byte;//未使用
  // LCDWF11=remotY11.Byte;//未使用
  LCDWF12 = remotY12.Byte; // 28
  LCDWF13 = remotY13.Byte; // 27
  LCDWF14 = remotY14.Byte; //  26
  LCDWF15 = remotY15.Byte; // 25
  LCDWF16 = remotY16.Byte; // 24
  LCDWF17 = remotY17.Byte; // 23
  LCDWF18 = remotY18.Byte; //	22
  LCDWF19 = remotY19.Byte; // 21
  LCDWF20 = remotY20.Byte; // 20
  LCDWF21 = remotY21.Byte; // 19
  LCDWF22 = remotY22.Byte; // 18
  LCDWF23 = remotY23.Byte; // 17
  LCDWF24 = remotY24.Byte; // 16
  LCDWF25 = remotY25.Byte; // 15
  LCDWF26 = remotY26.Byte; //   9
  LCDWF27 = remotY27.Byte; // 10
                           // LCDWF28=remotY28.Byte;//11  //特别注意，作为COM口的对应LCDWFXX不能置值
  // LCDWF29=remotY29.Byte;//  12//特别注意，作为COM口的对应LCDWFXX不能置值
  // LCDWF30=remotY30.Byte;//13 //特别注意，作为COM口的对应LCDWFXX不能置值
  // LCDWF31=remotY31.Byte;// 14//特别注意，作为COM口的对应LCDWFXX不能置值
}

void SET_LCD_ALL_ON(void) {
  LCDWF0 = 0xff;
  LCDWF1 = 0xff;
  LCDWF2 = 0xff;
  LCDWF3 = 0xff;
  LCDWF4 = 0xff;
  LCDWF5 = 0xff;
  LCDWF6 = 0xff;
  LCDWF7 = 0xff;
  // LCDWF8=0xff;
  // LCDWF9=0xff;
  // LCDWF10=0xff;      //特别注意，作为COM口的对应LCDWFXX不能置值
  // LCDWF11=0xff;      //特别注意，作为COM口的对应LCDWFXX不能置值
  LCDWF12 = 0xff; //特别注意，作为COM口的对应LCDWFXX不能置值
  LCDWF13 = 0xff; //特别注意，作为COM口的对应LCDWFXX不能置值
  LCDWF14 = 0xff;
  LCDWF15 = 0xff;
  LCDWF16 = 0xff;
  LCDWF17 = 0xff;
  LCDWF18 = 0xff;
  LCDWF19 = 0xff;
  LCDWF20 = 0xff;
  LCDWF21 = 0xff;
  LCDWF22 = 0xff;
  LCDWF23 = 0xff;
  LCDWF24 = 0xff;
  LCDWF25 = 0xff;
  LCDWF26 = 0xff;
  LCDWF27 = 0xff;
  // LCDWF28=0xff;   //特别注意，作为COM口的对应LCDWFXX不能置值
  // LCDWF29=0xff;    //特别注意，作为COM口的对应LCDWFXX不能置值
  // LCDWF30=0xff;    //特别注意，作为COM口的对应LCDWFXX不能置值
  // LCDWF31=0xff;    //特别注意，作为COM口的对应LCDWFXX不能置值
}

void disp_Tmode2(void) {

  LCDWF0 = 0x05; // 8
  LCDWF1 = 0x0a; // 7
  LCDWF2 = 0x05; // 6
  LCDWF3 = 0x0a; // 5
  LCDWF4 = 0x05; // 4
  LCDWF5 = 0x0a; // 3
  LCDWF6 = 0x05; // 2
  LCDWF7 = 0x0a; // 1
  // LCDWF8=remotY8.Byte;// 未使用
  // LCDWF9=remotY9.Byte;// 未使用
  // LCDWF10=remotY10.Byte;   //未使用
  // LCDWF11=remotY11.Byte;  //未使用
  LCDWF12 = 0x05; // 28
  LCDWF13 = 0x0a; // 27
  LCDWF14 = 0x05; //  26
  LCDWF15 = 0x0a; // 25
  LCDWF16 = 0x05; // 24
  LCDWF17 = 0x0a; // 23
  LCDWF18 = 0x05; //	22
  LCDWF19 = 0x0a; // 21
  LCDWF20 = 0x05; // 20
  LCDWF21 = 0x0a; // 19
  LCDWF22 = 0x05; // 18
  LCDWF23 = 0x0a; // 17
  LCDWF24 = 0x05; // 16
  LCDWF25 = 0x0a; // 15
  LCDWF26 = 0x0a; //   9
  LCDWF27 = 0x05; // 10
}

void disp_Tmode3(void) {

  LCDWF0 = 0x0a; // 8
  LCDWF1 = 0x05; // 7
  LCDWF2 = 0x0a; // 6
  LCDWF3 = 0x05; // 5
  LCDWF4 = 0x0a; // 4
  LCDWF5 = 0x05; // 3
  LCDWF6 = 0x0a; // 2
  LCDWF7 = 0x05; // 1
  // LCDWF8=remotY8.Byte;// 未使用
  // LCDWF9=remotY9.Byte;// 未使用
  // LCDWF10=remotY10.Byte;   //未使用
  // LCDWF11=remotY11.Byte;  //未使用
  LCDWF12 = 0x0a; // 28
  LCDWF13 = 0x05; // 27
  LCDWF14 = 0x0a; //  26
  LCDWF15 = 0x05; // 25
  LCDWF16 = 0x0a; // 24
  LCDWF17 = 0x05; // 23
  LCDWF18 = 0x0a; //	22
  LCDWF19 = 0x05; // 21
  LCDWF20 = 0x0a; // 20
  LCDWF21 = 0x05; // 19
  LCDWF22 = 0x0a; // 18
  LCDWF23 = 0x05; // 17
  LCDWF24 = 0x0a; // 16
  LCDWF25 = 0x05; // 15
  LCDWF26 = 0x05; //   9
  LCDWF27 = 0x0a; // 10
}

void disp_Tmode4(void) {

  disp_modecold = 1;
  disp_fanspword = 1;
  disp_fanspauto = 1;
  disp_fansp1 = 1;
  // disp_health=1;

  disp_fandirupword = 1;
  disp_fandirdownword = 1;
  disp_fandirmid = 1;

  // disp_swinglrword=1;
  // disp_swinglr2=1;
  // disp_swinglr4=1;

  disp_swingudword = 1;
  disp_swingud2 = 1;
  disp_swingud4 = 1;

  // disp_wifi=1;
  // disp_save=1;
  disp_light = 1;

  disp_batteryshell = 1;
  disp_battery1 = 1;
  disp_lock = 0;

  disp_sleep = 1;
  disp_sleepmodea = 1;
  disp_sleepmodeb = 1;
  disp_sleepmodec = 1;
  disp_sleepmoded = 1;
  disp_sleepmodeg = 1;

  disp_tempLl = tempTAB_eight[0x5e & 0x0f];
  disp_tempLh = tempTAB_eight[0x5e & 0x0f] >> 4;

  disp_tempHl = tempTAB_eight[(0x5e & 0xf0) >> 4];
  disp_tempHh = tempTAB_eight[(0x5e & 0xf0) >> 4] >> 4;

  disp_timerLl = timerTAB_eight[0x12 & 0x0f];
  disp_timerLh = timerTAB_eight[0x12 & 0x0f] >> 4;

  disp_timerHl = timerTAB_eight[(0x12 & 0xf0) >> 4];
  disp_timerHh = timerTAB_eight[(0x12 & 0xf0) >> 4] >> 4;

  disp_timerpoint = 1;
  disp_timerfivel = timerTAB_eight[5];
  disp_timerfiveh = timerTAB_eight[5] >> 4;
  disp_timerlater = 1;
  disp_timeron = 1;
}

void disp_Tmode5(void) {

  disp_modehot = 1;
  disp_fanspword = 1;
  disp_fansp1 = 1;
  disp_fansp2 = 1;
  disp_fansp3 = 1;

  disp_batteryshell = 1;
  disp_battery1 = 1;
  disp_battery2 = 1;

  disp_tubro = 1;
  // disp_wifi=1;
  disp_run = 1;
  disp_lock = 1;
  disp_tempunitC = 1;

  // disp_swinglrword=1;
  // disp_swinglr1=1;
  // disp_swinglr3=1;
  // disp_swinglr5=1;

  disp_swingudword = 1;
  disp_swingud1 = 1;
  disp_swingud3 = 1;
  disp_swingud5 = 1;

  disp_tempLl = tempTAB_eight[0x22 & 0x0f];
  disp_tempLh = tempTAB_eight[0x22 & 0x0f] >> 4;

  disp_tempHl = tempTAB_eight[(0x22 & 0xf0) >> 4];
  disp_tempHh = tempTAB_eight[(0x22 & 0xf0) >> 4] >> 4;

  disp_timerLl = timerTAB_eight[0x24 & 0x0f];
  disp_timerLh = timerTAB_eight[0x24 & 0x0f] >> 4;

  disp_timerHl = timerTAB_eight[(0x24 & 0xf0) >> 4];
  disp_timerHh = timerTAB_eight[(0x24 & 0xf0) >> 4] >> 4;

  disp_timerlater = 1;
  disp_timeroff = 1;
}

void disp_Tmode6(void) {

  disp_tempLl = tempTAB_eight[testkeynum & 0x0f];
  disp_tempLh = tempTAB_eight[testkeynum & 0x0f] >> 4;
  // if(testkeynum&0xf0)
  //	{
  disp_tempHl = tempTAB_eight[(testkeynum & 0xf0) >> 4];
  disp_tempHh = tempTAB_eight[(testkeynum & 0xf0) >> 4] >> 4;
  //	}
}

void disp_testmode(void) {

  dispclear();
  switch (testmode) {
  case 1:
    SET_LCD_ALL_ON();
    break;
  case 2:
    disp_Tmode3();
    break;
  case 3:
    disp_Tmode2();
    break;
  case 4:
    disp_Tmode4();
    dispset();
    break;
  case 5:
    disp_Tmode5();
    dispset();
    break;
  case 6:
    disp_Tmode6();
    disp_lock = 0;
    dispset();
    break;
  default:
    break;
  }
}

void disp_devtest(void) {

  switch (testmode) {
  case 1:
    //dispclear();
    disp_lock = 0;
    SET_LCD_ALL_ON();
    //delay3s(4);
    //devtest_exit();
    break;
  case 2:
    //dispclear();
    disp_lock = 0;
    disp_Tmode3();
    //delay3s(3);
    //devtest_exit();
    break;
  case 3:
    //dispclear();
    disp_lock = 0;
    disp_Tmode2();
   // delay3s(3);
   // devtest_exit();
    break;
  case 4:
    //dispclear();
    disp_lock = 0;
    disp_line = 1;
    disp_Tmode4();
    dispset();
    //delay3s(3);
    //devtest_exit();
    break;
  case 5:
    //dispclear();
    disp_lock = 0;
    disp_line = 1;
    disp_Tmode5();
    dispset();
    //delay3s(3);
   // devtest_exit();
    break;
  default:
    break;
  }
}

//显示主函数
void display(void) {
  
 if (displayflag == 0) {
    return;
 }
  displayflag = 0;

  dispclear();

  if ((devtestflag == 1) && (testmode > 0) && (testtime > 0)) {

    disp_devtest();
    if (testmode == 6) {

      disp_lock = 0;
      disp_Tmode6();
      dispset();
     
    }
     return;
  }

  if ((testflag != 0) && (testtime > 0)) {

    disp_testmode();
  }

  else {

    disptempnum();
    disptimer();
    displight();

    if (F0codeflag) {
      disp_run = 1;
    }

    if ((batflashcount & 0x01) == 0) {
      disbatteryvol(); //显示电池电量子程序
    }

    dispswingudtwinkle();

    if (onoffflag || ((timersta >= 1) && (timersta < 12))) {

      dispsleep();
      dispmode();
      dispfansp();
      dispfandir();
      dispswingud();
      dispkeepwarm();
      dispenjoywarm();
      dispslient();
      dispturbo();
    }
    disp_line = 1;
    dispset();
  }
}
