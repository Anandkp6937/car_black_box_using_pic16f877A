/*
 * File:   main.c
 * Author: ANAND
 *
 * Created on 19 December, 2025, 6:54 AM
 */


#include <xc.h>
#include "main.h"
#pragma config WDTE=OFF

unsigned char event[3]="ON";
char *gear[]={"GN","GR","G1","G2","G3","G4"};
 unsigned char gr=0;
  unsigned char res=0;
   unsigned int pot_value=0;
extern unsigned int reset;
void config(){
    init_clcd();
     init_adc();
      init_i2c(100000);
      init_ds1307();
      init_digital_keypad();
      init_timer0();
        init_uart(9600);
    puts("UART Test Code\n\r");
    GIE=1;
    PEIE=1;
}


//grab pot value to potVal array
 void getPotVal() {
    pot_value = read_adc();
    unsigned long temp;

    temp = (unsigned long)pot_value * 99;      
    unsigned char new_mapped_value = temp / 1023;
    potVal[0] = (new_mapped_value / 10) + '0';
    potVal[1] = (new_mapped_value % 10) + '0';
    potVal[2] = '\0';
    
}
 
void main(void) {
    config();
    
    unsigned char control_flag=HOMESCREEN_FLAG;
    unsigned char reset_flag=RESET_PW;
    unsigned char key,k2;
    unsigned char menu_pos=0;
    static int hold=0;
    
    while(1){
        
        
       
        key=read_digital_keypad(STATE);
         for(int j=3000;j--;);
        
         k2=read_digital_keypad(LEVEL);
         
         if(k2==SW5 && control_flag!=HOMESCREEN_FLAG){
             
             TMR0IE=1;
             
             if(reset){
                 clear_screen();
                  control_flag=HOMESCREEN_FLAG;
             }
             
             }
  
         else{
             reset=0;
               TMR0IE=0;
         }
         
        
         
         if(key==SW1){
             strcpy(event,"C ");
             log_car_event(event,potVal);
           
         } 
         else if(key== SW2 && gr < 5) {
              gr++;
            strcpy(event, gear[gr]);
            log_car_event(event,potVal);
         
        }
        else if(key== SW3 && gr > 0)
        {
            gr--;
            strcpy(event,gear[gr]);
            log_car_event(event,potVal);
          
        }
        else if(control_flag==HOMESCREEN_FLAG && (key==SW4 || key==SW5)&& control_flag!=LOGIN_SCREEN_FLAG){
        clear_screen();
            clcd_print("ENTER  PASSWORD ",LINE1(0));
            clcd_write(CURSOR_POS, INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
            __delay_us(100);
            control_flag=LOGIN_SCREEN_FLAG;
            reset_flag=RESET_;
        
        }
         else if(control_flag==MENU_FLAG && (key==SW6)){
            switch(menu_pos)
            {
                case 0:     //view log
                    clear_screen();
                    control_flag=VIEW_LOG_FLAG;
                        break;
                case 1:     //clear log
                    clear_screen();
                    control_flag=CLEAR_LOG_FLAG;
                    
                        break;
                case 2:     //change password
                   
                    clear_screen();
                     log_car_event("CP",potVal);
                    control_flag=CHANGE_PASSWORD_FLAG;
                        break;
                 
                  case 3:     //change password
                   
                    clear_screen();
                    control_flag=DOWNLOAD_FLAG;
                        break;       
                        
                 case 4:     //change time
                   
                    clear_screen();
                         log_car_event("CL",potVal);
                    control_flag=SET_TIME_FLAG;
                        break;       
                        
                        
            }
        }
        
        
        
        switch(control_flag){
            
            case HOMESCREEN_FLAG :
                disp_dashboard(event);
                break;
                
            case LOGIN_SCREEN_FLAG:
                switch(login(key,reset_flag)){
                
                    case RETURN_BACK:
                        control_flag=HOMESCREEN_FLAG;
                        TMR0IE = 0;
                        clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                        __delay_us(100);
                        break;
                        
                    case SUCCESS :
                            control_flag=MENU_FLAG;
                            reset_flag=RESET_;
                            clear_screen();
                            clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                            __delay_us(100);
                            TMR0IE = 0;
                                continue;
                            
                }
//                clcd_print("L",LINE2(0));
                break;
                
                
                
            case MENU_FLAG :
                menu_pos=login_menu(key);
                break;
                
            case VIEW_LOG_FLAG :
                view_log(key);
           
                break;
                
            case CHANGE_PASSWORD_FLAG :
                res=change_passwd(key);
                switch(res){
                    case SUCCESS:
                         clear_screen();
                        control_flag=MENU_FLAG;
                        break;   
                        
                    default:
                        continue;
                
                }
               
                break;
                
            case CLEAR_LOG_FLAG :
                clear_logs();
                clear_screen();
                control_flag=MENU_FLAG;
                
                break;
            
            case SET_TIME_FLAG:
              
              res=set_time(key);
              TMR0IE=1;
              switch(res){
                  case SUCCESS:
                      clear_screen();
                      control_flag=HOMESCREEN_FLAG;
                      break;
              }
                break;
                
                
            case DOWNLOAD_FLAG     :
                download_log();
        }
        
    
    }
    return;
}
