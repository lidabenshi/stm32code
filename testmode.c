#include "config.h"
#include "keyfunction.h"
#include "display.h"


void testpro() {

  displayflag=1;
  key_scan();
  
  if((currentkey!=0)&&(sendcodesta==0)) {

    backlight=0;
    backlightcounter=1;
    F0codeflag=1;
    
   
   if ((devtestflag == 1) && (testmode > 0) && (testmode < 6)) {

      devtestflag = 2;
      testmode = 0;
    
    }
  } 
  
  if((testflag!=0)||(devtestflag == 1)) {
     
     if(testmode!=0) {
        
       
        switch(currentkey) {
        
         case key_combine_onoff_mode_fansp:
            key_testpro();
            break;
          case key_minus:
            key_minuspro();
            break;
          case key_plus:
            key_pluspro();
            break;
          case key_on_off:
            key_onoffpro();
            break;
          default :
            break;
        }
       
        lastkey=currentkey;
        if(testmode==6) {
        
          
          switch(currentkey){
            
            //case key_on_off: testkeynum=0x01;F0codeflag=1;break;
            //case key_plus:   testkeynum=0x02;F0codeflag=1;break;
            //case key_mode:   testkeynum=0x03;F0codeflag=1;break;
            //case key_enjoywarm: testkeynum=0x04;F0codeflag=1;break;
            //case key_fansp:  testkeynum=0x05;F0codeflag=1;break;
            //case key_minus:  testkeynum=0x06;F0codeflag=1;break;
            //case key_cold:   testkeynum=0x07;F0codeflag=1;break;
            //case key_hot:    testkeynum=0x08;F0codeflag=1;break;
            //case key_swingud:testkeynum=0x09;F0codeflag=1;break;
            //case key_fandir: testkeynum=0x10;F0codeflag=1;break;
            //case key_silence:   testkeynum=0x11;F0codeflag=1;break;
            //case key_turbo:  testkeynum=0x12;F0codeflag=1;break;
            //case key_keepwarm: testkeynum=0x13;F0codeflag=1;break;
            //case key_light:  testkeynum=0x14;F0codeflag=1;break;
            //case key_timer:  testkeynum=0x15;F0codeflag=1;break;
            //case key_sleep:  testkeynum=0x16;F0codeflag=1;break;
            
            case key_on_off: testkeynum=0x01;break;
            case key_plus:   testkeynum=0x02;break;
            case key_mode:   testkeynum=0x03;break;
            case key_enjoywarm: testkeynum=0x04;break;
            case key_fansp:  testkeynum=0x05;break;
            case key_minus:  testkeynum=0x06;break;
            case key_cold:   testkeynum=0x07;break;
            case key_hot:    testkeynum=0x08;break;
            case key_swingud:testkeynum=0x09;break;
            case key_fandir: testkeynum=0x10;break;
            case key_silence:   testkeynum=0x11;break;
            case key_turbo:  testkeynum=0x12;break;
            case key_keepwarm: testkeynum=0x13;break;
            case key_light:  testkeynum=0x14;break;
            case key_timer:  testkeynum=0x15;break;
            case key_sleep:  testkeynum=0x16;break;
            default:
              break;
          }
        }
     }
  }
  
    
}





