#include "config.h"
#include "keyfunction.h"
#include "sendcode.h"

void setclockcommand(void) //ÿ15min ��f0��������Ӧ�ĵ���
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

  sleepflag = 0; //��˯��
  sleepmode = 0;
  sleepturbocounter = 0;
  sleeptwinklecounter = 0; //���˯�߼�ʱ��ر���

  timersta = 0x00;       //�ض�ʱ���ػ�����
  silentcount = 0x00;    //ȡ������
  silentcountbak = 0x00; /*�޾���*/

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
      tempvals[mode_temp] = 0x28; // 28���϶�����
      tempvalh[mode_temp] = 0x82; //�������϶�����
      
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
  } //��������ʱ��

  else {

    testkeytime = 0;
  }
  if (devtesttime > 0) {

    devtesttime--;
  } else {
    devtesttime = 0;
  }

  if (testtime > 0) //����ģʽ����ʱ��
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
  } //��˸��־
  else {

    charflash = 1;
  }

  if (backlightcounter != 0) //���������
  {
    backlightcounter++;
    if (backlightcounter >= 7) {
      backlightcounter = 0;
      backlight = 1;
    }
  }

  if (locktwinklecount != 0) //����˸
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

  if (swingcount != 0) //ɨ����˸�л�ɨ��ģʽʱ����˸
  {
    swingcount++;
    if (swingcount >= 9)
      swingcount = 1;
    displayflag = 1;
  }

  if (uddiscounter != 0) //����ɨ�����1~5��˸,6����ʾ
  {
    if (UDswingflash == 1) //����ɨ����˸ˢ��
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

  if (lrdiscounter != 0) //����ɨ�����1~5��˸,6����ʾ
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

  if (sleeptwinklecounter != 0) //˯��3�¶ȵ���
  {
    sleeptwinklecounter++;
    if (sleeptwinklecounter >= 21) {
      sleepturbocounter = 0;
      sleeptwinklecounter = 0;
    }
    displayflag = 1;
  }

  if ((HScounter >= 1) && (HScounter < H1Twinklecounter)) //��˪��˸
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

  if (batflashflag == 1) //�͵�����˸
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
  timercount();   //��ʱ����ʱ���ʱ
  batterycheck(); //�������

  enjoywarmcount();
}