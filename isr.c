#include <xc.h>
#include "main.h"

unsigned char ch;
extern char sec,return_time;
extern unsigned char key_timer;

void __interrupt() isr(void)
{
    static unsigned int count = 0;
    
    if (TMR2IF == 1)
    {
        if (++count == 1250)
        {
            count = 0;
            if(sec > 0)
            {
                sec--;
            }
            if(sec  == 0 && return_time > 0)
            {
                return_time--;
            }
            
           
        }
        
        TMR2IF = 0;
    }
    if (RCIF == 1)
    {
        if (OERR == 1)
            OERR = 0;
        
        ch = RCREG;
        
        RCIF = 0;
    }
}