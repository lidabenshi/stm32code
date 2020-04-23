#include "config.h"
#include "keyfunction.h"
#include "sendcode.h"

void setclockcommand(void) //每15min 对f0码做出相应的调整
{
  unsigned char timertemp;
  timerf0halfhcnt++;
  if (timerf0halfhcnt >= 30) {
    timerf0halfhcnt = 0x00;
    if (timerf0halfhourcode != 0x00) {
      timerf0halfhourcode = 0x00;
      return;
    }
    if (timerf0hourcode != 0x00) {
      timerf0halfhourcode = 0x50;
      timertemp = BCDToHex(timerf0hourcode);
      timertemp--;
      timerf0hourcode = HexToBCD(timertemp);
    }
  }
}

void onoffcomm(void) {

  sleepflag = 0; //关睡眠
  sleepmode = 0;
  sleepturbocounter = 0;
  sleeptwinklecounter = 0; //清除睡眠计时相关变量

  timersta = 0x00;       //关定时开关机设置
  silentcount = 0x00;    //取消静音
  silentcountbak = 0x00; /*无静音*/

  timeronoffs = 0x00;
  timeronoffminute = 0x0000;
  timerf0halfhcnt = 0x00;
  timerf0hourcode = 0x00;
  timerf0halfhourcode = 0x00;
  timeronoffs = 0x00;
  sethour = 0x00;
  sethalfhour = 0x00;
}

void onoff(void) {

  if (onoffflag) {

    onoffflag = 0;
    if (HScounter != 0) {

      if (onoffflag) {

        if (mode_temp == 0x04) {

          HScounter = 0x01;
          HSudflag = 0;
        } else {
          HScounter = H1Twinklecounter;
        }
      } else {

        HScounter = 0x01;
        HSudflag = 0;
      }
    }
    swingcount = 0x00;
    healthflag = 0;
    onoffcomm();
  } else {

    onoffflag = 1;

    if (HScounter != 0) {

      if (onoffflag) {

        if (mode_temp == 0x04) {

          HScounter = 0x01;
          HSudflag = 0;
        } else {
          HScounter = H1Twinklecounter;
        }
      } else {

        HScounter = 0x01;
        HSudflag = 0;
      }
    }
    if (swinglrsta[mode_temp] == 0x08) {

      swinglrsta[mode_temp] = 0x01;
    }

    else {
      swinglrsta[mode_temp] = 0x00;
    }

    onoffcomm();
    if ((lrdiscounter != 0) || (uddiscounter != 0)) {

      lrdiscounter = 0;
      uddiscounter = 0;
    }
  }
}
void timercount(void) {

  if (timersta != 0) {
    if (timersta == 12) {
      if (timeronoffminute == 0x00) {

        return;
      }
      timeronoffs++;
      if (timeronoffs == 120) // 1 min
      {
        timeronoffs = 0;
        timeronoffminute--;
        setclockcommand();
        timerhalfhourcnt++;
        if (timerhalfhourcnt >= 30) {
          timerhalfhourcnt = 0;
          timeronofftemp = settimerdown(timeronofftemp);
          displayflag = 1;

          if (sethalfhour == 0x00 && sethour == 0x24) {
            onoff();
          }
        }
      }

    } else {
      timersta++;
      if (timersta == 11) {
        timersta = 0;
        if (onoffflag == 0) {

          swingflag = 0;
        }
      }
      displayflag = 1;
    }
  }
}

/*20170717 DISPLAY DEVICE TEST MODE*/


/*20161017 add enjoywarm strong hot */
void enjoywarmcount(void) {

  if ((1 == enjoywarmmode) && (onoffflag == 1)) {

    senjoywarmtime++;

    if (120 == senjoywarmtime) { // 1min

      senjoywarmtime = 0;
      menjoywarmtime++;
    }

    if (30 == menjoywarmtime) {

      menjoywarmtime = 0;

      enjoywarmflag = 0;
      enjoywarmmode = 0;

      mode_temp = 4;
      tempvals[mode_temp] = 0x28; // 28摄氏度设置
      tempvalh[mode_temp] = 0x82; //华氏摄氏度设置
      
      displayflag = 1;
    }

  } else {

    menjoywarmtime = 0;
    senjoywarmtime = 0;
  }
  
}


/*
void devtest_exit(void) {
 
    key_scan();
    if ((currentkey != 0) && (testmode > 0) && (testmode < 6)) {

      devtestflag = 2;
      testmode = 0;
    }
    else 
    {
      testmode++;
    }

  
} */

void systimepro(void) {

  if (testkeytime > 0) {

    testkeytime--;
  } //开机运行时间

  else {

    testkeytime = 0;
  }
  if (devtesttime > 0) {

    devtesttime--;
  } else {
    devtesttime = 0;
  }

  if (testtime > 0) //测试模式运行时间
  {
    testtime--;
    if(testtime == 0)
    {
       displayflag = 1;
    }
  } else {

    testtime = 0;
    testflag = 0;
    devtestflag = 0;
  }

  if (charflash == 1) {

    charflash = 0;
  } //闪烁标志
  else {

    charflash = 1;
  }

  if (backlightcounter != 0) //背光灯三秒
  {
    backlightcounter++;
    if (backlightcounter >= 7) {
      backlightcounter = 0;
      backlight = 1;
    }
  }

  if (locktwinklecount != 0) //锁闪烁
  {
    disp_lock = 0;
    if ((locktwinklecount & 0x01) == 0) {
      disp_lock = 1;
    }

    locktwinklecount--;
    displayflag = 1;
  } else {

    if (lockflag == 1) {
      disp_lock = 1;
      displayflag = 1;
    }
  }

  if (swingcount != 0) //扫风闪烁切换扫风模式时候闪烁
  {
    swingcount++;
    if (swingcount >= 9)
      swingcount = 1;
    displayflag = 1;
  }

  if (uddiscounter != 0) //上下扫风计数1~5闪烁,6关显示
  {
    if (UDswingflash == 1) //上下扫风闪烁刷新
    {
      UDswingflash = 0;
    } else {
      UDswingflash = 1;
    }

    uddiscounter++;
    if (uddiscounter >= 5) {
      uddiscounter = 0;
    }
    displayflag = 1;
  }

  if (lrdiscounter != 0) //左右扫风计数1~5闪烁,6关显示
  {
    if (LRswingflash == 1) {
      LRswingflash = 0;
    } else {
      LRswingflash = 1;
    }

    lrdiscounter++;
    if (lrdiscounter >= 5) {
      lrdiscounter = 0;
    }
    displayflag = 1;
  }

  if (sleeptwinklecounter != 0) //睡眠3温度调整
  {
    sleeptwinklecounter++;
    if (sleeptwinklecounter >= 21) {
      sleepturbocounter = 0;
      sleeptwinklecounter = 0;
    }
    displayflag = 1;
  }

  if ((HScounter >= 1) && (HScounter < H1Twinklecounter)) //化霜闪烁
  {
    if (H1twinkleflag == 1) {

      H1twinkleflag = 0;
    }

    else {

      H1twinkleflag = 1;
    }

    HScounter++;
    if (HScounter >= H1Twinklecounter) {

      HScounter = H1Twinklecounter;
    }

    displayflag = 1;
  }

  if (batflashflag == 1) //低电量闪烁
  {

    if (batflashcount > 0) {
      batflashcount--;
    } else {
      batflashcount = 0;
      batflashflag = 0;
    }
    displayflag = 1;
  }
 
  if((devtestflag == 1)&& (testmode > 0) && (testmode < 6)){
  
    devtestcout++;
    
    if(testmode == 1){
    
      if(devtestcout==4){
        testmode++;
        devtestcout=0;
        displayflag=1;
      }
    }else{
    
       if(devtestcout==3){
        testmode++;
        devtestcout=0;
        displayflag=1; 
      }
    }
  } 
  timercount();   //定时开关时间计时
  batterycheck(); //电量检测

  enjoywarmcount();
}