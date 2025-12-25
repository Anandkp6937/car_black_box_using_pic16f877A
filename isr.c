#include <xc.h>
#include "main.h"

unsigned char return_time,sec=0;
volatile unsigned char blink=0;
volatile int hold=0;
unsigned int reset=0;
void __interrupt() isr(void)
{      
    static unsigned counter=0,blinker=0;

    if (TMR0IF == 1)
    {
        TMR0 = TMR0 + 8;      
        
        if(hold++==30000){
            reset=1;
            hold=0;
        }
        
        
        
        
        
        
        if(++blinker==10000){
        blink=!blink;
        blinker=0;
        
        }
            
        
        
        if(++counter==20000){
            
            
            
            counter=0;
            if(sec>0){
                sec--;
            }
            
            else if(sec==0 && return_time >0)
            return_time--;
            
            
            
        }
        
       
        TMR0IF = 0;       
    }
}