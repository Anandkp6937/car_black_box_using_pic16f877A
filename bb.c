#include "main.h"
#include <xc.h>

#define _XTAL_FREQ 20000000

unsigned char clock_reg[3];
char time[7];
volatile unsigned char potVal[4];
extern unsigned char return_time;
extern volatile unsigned char blink;
extern unsigned char sec;
unsigned char log[11];
unsigned char loc=0;
unsigned char next=0;
int event_count=-1;
  static int Donce=0;
char pass[5]="1212";

char *menu[]={"View log","Clear log","Change passwrd","Download log","Set time"};

//grab time
void get_time(void)
{
    
    clock_reg[0]=read_ds1307(HOUR_ADDR);
    clock_reg[1]=read_ds1307(MIN_ADDR);
    clock_reg[2]=read_ds1307(SEC_ADDR);
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] ='\0';
}

void display_time()
{
    get_time();
    clcd_putch(time[0],LINE2(0));
    clcd_putch(time[1],LINE2(1));
    clcd_putch(':',LINE2(2));
    clcd_putch(time[2],LINE2(3));
    clcd_putch(time[3],LINE2(4));
    clcd_putch(':',LINE2(5));
    clcd_putch(time[4],LINE2(6));
    clcd_putch(time[5],LINE2(7));
} 
 //function to clear screen
void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
   __delay_us(100);
}
 
 //display dashboard
 void disp_dashboard(char event []){
     display_time();
    getPotVal();
    clcd_print(" TIME     E  SP",LINE1(0));
    clcd_print(event, LINE2(10));
    clcd_print(potVal,LINE2(13));

 
 }

 //login 
 static int once=0;
 char login(unsigned char key, unsigned char reset_flag)
{
    static char uPass[5];
    static unsigned char i;
    static unsigned char attempt;

    if(once == 0)
    {
        return_time = 5;
        attempt = '3';
        i = 0;
        for(int k=0;k<4;k++)
            uPass[k] = '\0';
        key = ALL_RELEASED;
        once=1;
    }

    if(return_time == 0){
        once=0;
        return RETURN_BACK;
    }
    if(key == SW4 && i < 4)
    {
        uPass[i] = '1';
        clcd_putch('*', LINE2(6 + i));
        i++;
        return_time = 5;
    }
    else if(key == SW5 && i < 4)
    {
        uPass[i] = '2';
        clcd_putch('*', LINE2(6 + i));
        i++;
        return_time = 5;
    }

    if(i == 4)
    {
        if(strcmp(uPass,pass)==0){
        return SUCCESS;
        }
        else{
            attempt--;
            
            if(attempt=='0')
            {
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                __delay_us(100);
                clcd_print(" You are blocked",LINE1(0));
                clcd_print("Wait...for 60sec",LINE2(0));
                sec=60;
                
                  while(sec!=0)
                {
                    clcd_putch((sec/10) + '0', LINE2(11));
                    clcd_putch((sec%10) + '0', LINE2(12));
                    
                }
                attempt='3';
                
        }
             else{
                clear_screen();
                
                clcd_print(" WRONG PASSWORD ",LINE1(0));
                clcd_putch(attempt,LINE2(0));
                clcd_print(" attempt remain",LINE2(1));
                __delay_ms(2000);
            }
            clear_screen();
            clcd_print(" ENTER  PASSWORD ",LINE1(0));
            clcd_write(CURSOR_POS, INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
            __delay_us(100);
            i=0;
            return_time=5;
        } 
        
          
    }

    return 0x10;  
 }

 
 //login menu 
 char login_menu(unsigned char key)
{
    static unsigned char menu_pos = 0;   // menu index
    static unsigned char pos = 0;       

    if (key == SW5)   // DOWN
    {
        if (pos < 1)
        {
            pos++;
        }
        else if (menu_pos < 3)
        {
            menu_pos++;
        }
         clear_screen();
    }
    else if (key == SW4)   // UP
    {
        if (pos > 0)
        {
            pos--;
        }
        else if (menu_pos > 0)
        {
            menu_pos--;
        }
         clear_screen();
    }

   

    clcd_print(menu[menu_pos], LINE1(1));
    clcd_print(menu[menu_pos + 1], LINE2(1));

    clcd_putch(pos == 0 ? '*' : ' ', LINE1(0));
    clcd_putch(pos == 1 ? '*' : ' ', LINE2(0));

    return menu_pos + pos;
}
//view log
 char view_log(unsigned char key)
{
    static unsigned char logData[11]; 
    unsigned char add = 0;


    /* No logs present */
    if (event_count == -1)
    {
        clcd_print("# TIME     E  SP", LINE1(0));
        clcd_print("No logs present", LINE2(0));
        return SUCCESS;
    }

    if (key == SW5 && next < event_count)
        next++;
    else if (key == SW4 && next > 0)
        next--;

    if (next > 9) next = 0;
    if (next < 0) next = 0;

    add = next * 10;
    for (int i = 0; i < 10; i++)
    {
        logData[i] = read_external_eeprom(add + i);
    }
    logData[10] = '\0'; 

    clcd_print("# TIME     E  SP", LINE1(0));

  
    clcd_putch((next+'0'), LINE2(0));
    /* Display time HH:MM:SS */
    clcd_putch(logData[0], LINE2(2));
    clcd_putch(logData[1], LINE2(3));
    clcd_putch(':', LINE2(4));
    clcd_putch(logData[2], LINE2(5));
    clcd_putch(logData[3], LINE2(6));
    clcd_putch(':', LINE2(7));
    clcd_putch(logData[4], LINE2(8));
    clcd_putch(logData[5], LINE2(9));

    /* Display event and speed */
    clcd_putch(logData[6], LINE2(11));
    clcd_putch(logData[7], LINE2(12));
    clcd_putch(logData[8], LINE2(14));
    clcd_putch(logData[9], LINE2(15));

    return SUCCESS;
}

  
  //change password
   char change_passwd(unsigned char key){
       
       static unsigned char upass[9];
       static int i=0;
       static int sonce=0,once=0,pos=0;
       
       if(once==0){
           clear_screen();
        clcd_print("Enter New PassWd",LINE1(0));
        clcd_write(LINE2(5), INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
        once=1;
       }
       
       if(key==SW4 && i<8){
           clcd_putch('1',LINE2(pos+5));
           upass[i]='1';
           i++;
           pos++;
          
       }
       else if(key==SW5 && i<8){
           clcd_putch('2',LINE2(pos+5));
           upass[i]='2';
           i++;
           pos++;
           
       }
       
       if(i==4 && sonce==0){
            clear_screen();
        clcd_print("Re Enter PassWd",LINE1(0));
        clcd_write(LINE2(5), INST_MODE);
        clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
        sonce=1;
        pos=0;
       }
       
       if(i==8){
            clcd_write( DISP_ON_AND_CURSOR_OFF,INST_MODE);
           
           if(strncmp(upass,&upass[4],4)==0){
             clear_screen();
              clcd_print("PassWd change ",LINE1(0));
              clcd_print("Sucessful ",LINE2(0));
              pass[0]=upass[0];
              pass[1]=upass[1];
              pass[2]=upass[2];
              pass[3]=upass[3];
              
              __delay_ms(2000);
              return SUCCESS;
           }
           else{
           clear_screen();
              clcd_print("PassWd change",LINE1(0));
              clcd_print("Faild",LINE2(0));
              __delay_ms(2000);
              return SUCCESS;
           }
           
       }
       
   
   }
   
   //log car event
  
void log_car_event(char event[3], char speed[3])
{
    unsigned char add = 0;
    
    get_time();

    strncpy(log, time, 6);
    strncpy(&log[6], event, 2);
    strncpy(&log[8], speed, 2);

    log[10] = '\0'; 


    if (loc >= 10)
    {
        loc = 0;
    }

    add = 0x00 + (loc * 10);

    if (event_count < 9)
    {
        event_count++;
    }

    ext_eeprom_store_str(add, log);

    loc++;   
}


//decimal to bcd convertor 
unsigned char dec_to_bcd(unsigned char val)
{
    return ((val / 10) << 4) | (val % 10);
}



   //clear logs

void clear_logs(void){
    
    
    if(event_count>=0){
    event_count=-1;
    clcd_print("cleared logs..",LINE1(0));
    __delay_ms(1000);
    
    }
    else{
    
    clcd_print("nothing to clear ..",LINE1(0));
    __delay_ms(1000);
    }
    

}

//set time 

char set_time(unsigned char key)
{
    char hh[3], mm[3], ss[3];
    static unsigned char field = 0;   
    static unsigned char edited = 0;   
    int hour, min, sec;

  
    strncpy(hh, time, 2); hh[2] = '\0';
    strncpy(mm, &time[2], 2); mm[2] = '\0';
    strncpy(ss, &time[4], 2); ss[2] = '\0';

   
    hour = (hh[0] - '0') * 10 + (hh[1] - '0');
    min  = (mm[0] - '0') * 10 + (mm[1] - '0');
    sec  = (ss[0] - '0') * 10 + (ss[1] - '0');

  

 
    if (key == SW5)
    {
        field++;
        if (field > 2)
            field = 0;
    }

    /* SW4 ? Edit selected field */
    if (key == SW4)
    {
        edited = 1;   // mark as edited

        if (field == 0)            // SS
        {
            sec++;
            if (sec > 59) sec = 0;
        }
        else if (field == 1)       // MM
        {
            min++;
            if (min > 59) min = 0;
        }
        else if (field == 2)       // HH
        {
            hour++;
            if (hour > 23) hour = 0;
        }
    }

    /* SW6 ? Save ONLY if edited */
    if (key == SW6 && edited == 1)
    {
        i2c_start();
        i2c_write(0xD0);          // RTC write
        i2c_write(SEC_ADDR);      // seconds register

        i2c_write(dec_to_bcd(sec) & 0x7F); // seconds
        i2c_write(dec_to_bcd(min));        // minutes
        i2c_write(dec_to_bcd(hour));       // hours (24h)

        i2c_stop();

        edited = 0;   // reset edit state

        clear_screen();
        clcd_print("SUCCESSFULL", LINE1(0));
        __delay_ms(2000);
        return SUCCESS;
    }

    /* -------- UPDATE TIME STRING -------- */
    hh[0] = (hour / 10) + '0';
    hh[1] = (hour % 10) + '0';

    mm[0] = (min / 10) + '0';
    mm[1] = (min % 10) + '0';

    ss[0] = (sec / 10) + '0';
    ss[1] = (sec % 10) + '0';

    time[0] = hh[0]; time[1] = hh[1];
    time[2] = mm[0]; time[3] = mm[1];
    time[4] = ss[0]; time[5] = ss[1];

    /* -------- DISPLAY -------- */
    clcd_print("TIME  (HH:MM:SS)", LINE1(3));

    // HH
    if (field == 2 && !blink)
        clcd_print("  ", LINE2(0));
    else
        clcd_print(hh, LINE2(0));

    clcd_putch(':', LINE2(3));

    // MM
    if (field == 1 && !blink)
        clcd_print("  ", LINE2(5));
    else
        clcd_print(mm, LINE2(5));

    clcd_putch(':', LINE2(8));

    // SS
    if (field == 0 && !blink)
        clcd_print("  ", LINE2(10));
    else
        clcd_print(ss, LINE2(10));

 
}

//download log
static unsigned int next2=0;

char download_log(void){

     unsigned char logData[11];
     unsigned char add=0;
     static int tempF;
   
     tempF=event_count;
     
     
      if(event_count!=-1 ){
      
     if(!Donce){    
     while(tempF>=0){
         
      for(int i=0;i<10;i++){
      logData[i]=read_external_eeprom(add+(next2*10));
      add++;
 
      }
      
     logData[10]='\0';    
     puts(logData);
     puts("\n");
     tempF--;
     next2++;
     
     
     }    
     
    
     Donce=1;
      }
     
      clcd_print("Downloaded log",LINE1(0));
     
      }
      else{
          if(!Donce){
            puts("NO LOGS TO DOWNLOAD !");
            clcd_print("No logs to dwnload !",LINE1(0));
            Donce=1;
              
          }
          
          return SUCCESS;
            
      }
     
     
     
     
     
     
     
     
     
  
     }
     

