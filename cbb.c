/*
 * File:   cbb.c
 * Author: MUBASHSHIR
 *
 * Created on 7 August, 2023, 8:45 PM
 */


#include "main.h"


extern unsigned char control_flag;
unsigned char clock_reg[3];
 unsigned char view_log_arr[10]; 
unsigned char log[11],viewlog;
unsigned char time[7],time1[7];
char return_time = 5;
char pos = -1,pos1 = -1;
int pos2 = -1;
char sec; 
char overflow = 0;
 static int num = 0;
//extern unsigned char key;

void get_time()
{
    clock_reg[0] = read_ds1307(HOUR_ADDR); // HH -> BCD 
    clock_reg[1] = read_ds1307(MIN_ADDR); // MM -> BCD 
    clock_reg[2] = read_ds1307(SEC_ADDR); // SS -> BCD 
    
     
    // HH -> 
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time[1] = (clock_reg[0] & 0x0F) + '0';
    
    
    // MM 
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time[3] = (clock_reg[1] & 0x0F) + '0';
    
   
    // SS
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time[5] = (clock_reg[2] & 0x0F) + '0';
    time[6] = '\0';
    
     time1[0] = ((clock_reg[0] >> 4) & 0x03) + '0';
    time1[1] = (clock_reg[0] & 0x0F) + '0';
    
    
    // MM 
    time1[2] = ((clock_reg[1] >> 4) & 0x07) + '0';
    time1[3] = (clock_reg[1] & 0x0F) + '0';
    
   
    // SS
    time1[4] = ((clock_reg[2] >> 4) & 0x07) + '0';
    time1[5] = (clock_reg[2] & 0x0F) + '0';
    time1[6] = '\0';
}


void display_time(void)
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

void display_dashboard(char *event, char speed)
{
    clcd_print("  TIME     E  SP",LINE1(0));
    clcd_print(event,LINE2(11));
    
    if(strcmp(event,"ON") == 0)
    {
        speed = 0;
    }
    clcd_putch(speed/10 + '0',LINE2(14));
    clcd_putch(speed%10 + '0',LINE2(15));
    display_time();
    //clcd_print
}

void log_event(void)
{
    char addr = 0x05;
    pos++;
    if(pos == 10)
    {
        pos = 0;
        overflow = 1;
    }
    addr = pos * 10 + addr;
    eeprom_at24c04_str_write(addr,log);
    
    if(pos2 < 9)
    {
        pos2++;
    }
        
    
}


unsigned char view_log(unsigned char key,unsigned char reset_flag, unsigned char control_flag)
{
    //unsigned char pre_key ;//= read_digital_keypad(STATE);
    static unsigned char addr = 0x05;  // Static variable to remember EEPROM address
     // Changed array size to 10 for 10 logged strings
    static unsigned char num = 0;  
    int new;// Static variable to remember the current log number
    
    if (pos2 == -1)
    {
        //clear_screen();
       
        return FAILURE;
    }
    else
    {
       
        if (reset_flag == RESET_VIEW_LOG_FLAG)
        {
            num = 0;
            addr = 0x05;  
        }
        new = (overflow)? (pos + 1) % 10 : 0;

            
             for (int i = 0; i < 10; i++)
            {
                 addr = new*10 + 0x05;
                view_log_arr[i] = eeprom_at24c04_randon_read(addr + i);
            }
            
           }


            // Display the logged strings on both lines of the CLCD
            
                
            if(key == SW1 && control_flag == VIEW_LOG)
            {
                if(num < 9)
                {
                num = num+1 ;
                }
                else if(num == 9)
                {
                    num = 0;
                }
                
                
                //addr = 0x05 + (num*10);
            }
            else if(key == SW2 && control_flag == VIEW_LOG)
            {
                if(num > 0)
                {
                num = num-1;
                }
                else if(num == 0)
                {
                    num = 9;
                }
                //addr = 0x05 + (num*10);
                
            }
    new = (new+num)%10;
            if(key == LPSW5 && control_flag == VIEW_LOG)
            {
                control_flag = LOGIN_MENU_FLAG;
            }
                
    
            clcd_print("LOGS",LINE1(4));
            clcd_putch(num + '0', LINE2(0));            
            clcd_putch(view_log_arr[0], LINE2(3));
            clcd_putch(view_log_arr[1], LINE2(4));
            clcd_putch(view_log_arr[2], LINE2(5));
            clcd_putch(view_log_arr[3], LINE2(6));
            clcd_putch(view_log_arr[4], LINE2(7));
            clcd_putch(view_log_arr[5], LINE2(8));
            clcd_putch(view_log_arr[6], LINE2(9));
            clcd_putch(view_log_arr[7], LINE2(10));
            clcd_putch(view_log_arr[8], LINE2(11));
            clcd_putch(view_log_arr[9], LINE2(12));
   
    // Add a delay before exiting the function
    for(unsigned long int i = 0; i < 10000; i++);
    clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
    
}




unsigned char change_pwd(unsigned char key, unsigned char reset_flag,unsigned char control_flag)
{
   // unsigned char key = read_digital_keypad();
    char pwd[4];
    char pwd1[4];
    static int i = 0,flag = 0 ,j = 0;
    static int s = 0;
    
    if(reset_flag == RESET_NEW_PWD_FLAG && i < 4 && flag == 0)
    {
    clcd_print(" ENTER NEW PWD",LINE1(1));
     clcd_write(CURSOR_POS,INST_MODE);
    clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
    __delay_us(100);  
    }
            if(key == SW1 && i < 4  && flag == 0)
            {
                if(i == 3)
                {
                    key = ALL_RELEASED;
                }
                pwd[i] = '4';
                clcd_putch('*',LINE2(6+i));
                i++;
                //j++;
                return_time = 5;
            }
            else if(key == SW2 && i < 4 && flag == 0)   
            {
                if(i == 3)
                {
                    key = ALL_RELEASED;
                }
                pwd[i] = '5';
                clcd_putch('*',LINE2(6+i));
                
                i++;
                //j++;
                
                return_time = 5;
            }
    if( i == 4 && flag == 0)
    {
       clear_screen();
        reset_flag = RESET_NEW_PWD_FLAG2;
        flag = 1;
        //i = 0;
        
    }
    
   
    if(key == ALL_RELEASED && reset_flag == RESET_NEW_PWD_FLAG2 && i== 4 && s < 4 && flag == 1)
    {
           
    clcd_print("REENTER NEWPWD",LINE1(1));
     clcd_write(CURSOR_POS,INST_MODE);
    clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
    __delay_us(100);  
    }
            if(key == SW1 && i == 4 && s < 4 && flag == 1)
            {
                pwd1[s] = '4';
                clcd_putch('*',LINE2(6+s));
                s++;
                return_time = 5;
            }
            else if(key == SW2 && i == 4 && s < 4 && flag == 1)   
            {
                pwd1[s] = '5';
                clcd_putch('*',LINE2(6+s));
                s++;
                return_time = 5;
            }
   
    
    if(s == 4 && i == 4)
    {
        clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
        if(strncmp(pwd,pwd1,4) == 0)
        {
            clear_screen();
            i = 0;
            s = 0;
           // j=0;
            flag = 0;
            eeprom_at24c04_str_write(0x00,pwd);
                 return PWD_CHANGED;
          
        }
        else
        {
            clear_screen();
            i = 0;
            s = 0;
           // j=0;
            flag = 0;
           // reset_flag =  RESET_NEW_PWD_FLAG;
            
            return PWD_NOT_CHANGED;
     
        }
           
    }

}

unsigned char clear_log(unsigned char reset_flag)
{
    if(reset_flag == RESET_CLEAR_LOG)
    {
        pos = -1;
        pos2 = -1;
        return LOGS_CLEARED;
    }
    
    
    
}

unsigned char set_time(unsigned char key,unsigned char control_flag)
{
    static char flag = 0;
    static char position = 2;
    static unsigned char blink = 1,wait = 0;
    static char set_time_arr[3];
    static char hour_count1 = 0 ,hour_count2 = 0 ,min_count = 0 ,sec_count = 0,temp,temp1;
    static char hour_limit = 0,hour_limit2 = 0;
    char dummy;
    
    clcd_print("HH:MM:SS",LINE1(4));
    if(flag == 0)
    {
        
    get_time();
   // position = 2;
    set_time_arr[0] = (time[0] & 0x0F)*10 + (time[1] & 0x0F);
    set_time_arr[1] = (time[2] & 0x0F)*10 + (time[3] & 0x0F);
    set_time_arr[2] = (time[4] & 0x0F)*10 + (time[5] & 0x0F);
   
    
    flag = 1;
    }
    if(position == 3 )
    {
        position = 0;
    }
    if(position < 0 )
    {
        position = 2;
    }
    if(key == SW4 && flag == 1 )
    {
        set_time_arr[position]++;
    }
    else if(key == SW2 && flag == 1 && position < 3)
    {
        position++;
        
    }
    else if(key == SW5 && flag == 1 && position < 3)
    {
        set_time_arr[position]--;
    }
    else if(key == SW3 && flag == 1)
    {
        dummy = ((set_time_arr[0]/10) << 4 | (set_time_arr[0]%10));
        clock_reg[0] = (clock_reg[0] & 0xc0) | dummy;
        write_ds1307(HOUR_ADDR,clock_reg[0]);
        
        dummy = ((set_time_arr[1]/10) << 4 | (set_time_arr[1]%10));
        clock_reg[1] = (clock_reg[1] & 0x80) | dummy;
        write_ds1307(MIN_ADDR,clock_reg[1]);
        
        dummy = ((set_time_arr[2]/10) << 4 | (set_time_arr[2]%10));
        clock_reg[2] = (clock_reg[2] & 0xc0) | dummy;
        write_ds1307(SEC_ADDR,clock_reg[2]);
        
        flag = 2;
    }
    if(flag == 2)
    {
         clear_screen();
        return SUCCESS;
        
    }
    
    if(set_time_arr[0] > 23)
    {
        set_time_arr[0] = 0;
    }
    else if(set_time_arr[0] < 0)
    {
        set_time_arr[0] = 23;
    }
    
    if(set_time_arr[1] > 59)
    {
        set_time_arr[1] = 0;
    }
    else if(set_time_arr[1] < 0)
    {
        set_time_arr[1] = 59;
    }
    
    if(set_time_arr[2] > 59)
    {
        set_time_arr[2] = 0;
    }
    else if(set_time_arr[2] < 0)
    {
        set_time_arr[2] = 59;
    }
    
    if(++wait == 10)
    {
        wait = 0;
        blink = !blink;
    
   
    clcd_putch(set_time_arr[0] / 10 + '0',LINE2(4));
    clcd_putch(set_time_arr[0] % 10 + '0',LINE2(5));
    clcd_putch(':',LINE2(6));
    clcd_putch(set_time_arr[1] / 10 + '0',LINE2(7));
    clcd_putch(set_time_arr[1] % 10 + '0',LINE2(8));
    clcd_putch(':',LINE2(9));
    clcd_putch(set_time_arr[2] / 10 + '0',LINE2(10));
    clcd_putch(set_time_arr[2] % 10 + '0',LINE2(11));
    
    if(blink)
    {
        switch(position)
        {
            case 0:
					clcd_print("  ",LINE2(4));
					break;
				case 1:
					clcd_print("  ",LINE2(7));
					break;
				case 2:
					clcd_print("  ",LINE2(10));
					break;
        }
    }
    } 
}

void download_log(void)
{
    const char *s;
    unsigned char addr = 0x05;
    static int num = -1;
    int new = 0;
    unsigned char download_log_arr[11];
    download_log_arr[10] = 0;
    
    //download_log_arr[10] = 0;
    if (pos2 == -1)
    {
        putss("NO LOGS PRESENT");
       
    }
   else
    {
        putss("Logs :");
		putcharr('\n');
		putcharr('\n');
		putcharr('\r');

        while(num < pos2)
           {
              num++;
              new = (overflow)? (pos + 1) % 10 : 0;
              new = (new + num)%10;
               
           
             for (int i = 0; i < 10; i++)
            {
                 addr = new * 10 + 0x05;
                download_log_arr[i] = eeprom_at24c04_randon_read(addr + i);
                
            }
              
              
               putcharr(num + '0');
               putcharr(' ');              
               putcharr(download_log_arr[0]);
               putcharr(download_log_arr[1]);
               putcharr(':');
               putcharr(download_log_arr[2]);
               putcharr(download_log_arr[3]);
               putcharr(':');
               putcharr(download_log_arr[4]);
               putcharr(download_log_arr[5]);
               putcharr(' ');
               putcharr(download_log_arr[6]);
               putcharr(download_log_arr[7]);
               putcharr(' ');
               putcharr(download_log_arr[8]);
               putcharr(download_log_arr[9]);                           
               putcharr ( '\n' );
               putcharr ('\r');
              
           }
 
               
}  
    
}


void log_car_event(char *event, char speed)
{
    get_time();
    
    strncpy(log,time,6);
    strncpy(&log[6],event,2);
    
    log[8] = speed/10 + '0';
    log[9] = speed % 10 + '0';
    log[10] = '\0';
    
    log_event();
}

void clear_screen(void)
{
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
    __delay_us(500);
}

unsigned char login(unsigned char key , unsigned char reset_flag)
{
    static char attempts_rem = 3,i=0;
    static char npassword[4],spassword[4];
    char count = 0;
    
    if(reset_flag == RESET_PASSWORD)
    {
        attempts_rem = 3;
        i=0;
        npassword[0] = '\0';
        npassword[1] = '\0';
        npassword[2] = '\0';
        npassword[3] = '\0';
      // npassword[4] = '\0';
        
        key = 0xFF;
        return_time = 5;
    }
    
    if(key == SW4 && i < 4)
    {
        npassword[i] = '4';
        clcd_putch('*',LINE2(6+i));
        
        i++;
        return_time = 5;
    }
    else if(key == SW5 && i < 4)
    {
        npassword[i] = '5';
        clcd_putch('*',LINE2(6+i));
        
        i++;
        return_time = 5;
    }
   // npassword[4] = '\0';
    if(i == 4)
    {
        for(char j=0;j<4;j++)
        {
            spassword[j] =  eeprom_at24c04_randon_read(j);
        }
      //  spassword[4] = '\0';
        if(strncmp(npassword,spassword,4) == 0)
        {
            return LOGIN_SUCCESS;
            sec = 60;
            while(sec);
        }
        else
        {
            attempts_rem--;
           if(attempts_rem == 0)
           {
              sec = 60;
              clear_screen();
              clcd_print("YOU ARE BLOCKED",LINE1(0));
              clcd_print("WAIT FOR ",(LINE2(0)));
              clcd_print("SEC",LINE2(12));
               
            while(sec)
            {
                clcd_putch(sec/10 + '0',LINE2(9));
                clcd_putch(sec%10 + '0',LINE2(10));
            }
            attempts_rem = 3;  
               
           }
           else
           {
               
           clear_screen();
           
            clcd_print("Wrong Password",LINE1(0));
            clcd_putch(attempts_rem + '0',LINE2(0));
            clcd_print("Attempt Remain",LINE2(2));
            __delay_ms(2000);
                    
            clear_screen();
            clcd_print("ENTER PASSWORD",LINE1(1));
            i=0;
            return_time = 5;
            clcd_write(CURSOR_POS,INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
            __delay_us(100);
            
           }
        }
        
    }
    if(return_time == 0)
    {
        return RETURN_BACK;
    }
    
} 
  


unsigned char login_menu(unsigned char key, unsigned char reset_flag)
{
    
    char *menu[] = {"View Log","Clear Log","Download Log","Change PWD","Set Time"};
    static char menu_pos = 0;
    
    if(reset_flag == RESET_LOGIN_MENU)
    {
        clear_screen();
        menu_pos = 0;
    }
    if(key == SW4 && menu_pos > 0 )
    {
        menu_pos--;
        clear_screen();
    }
    else if(key == SW5 && menu_pos <= 3)
    
    {
        menu_pos++;
        clear_screen();
    }
    
    if(menu_pos < 4)
    {
        clcd_putch('*',LINE1(0));
    clcd_print(menu[menu_pos],LINE1(2));
    clcd_print(menu[menu_pos + 1],LINE2(2));
        
        
        
    }
    else if(menu_pos == 4 )
    {
        clcd_putch('*',LINE2(0));
        clcd_print(menu[menu_pos - 1],LINE1(2));
    clcd_print(menu[menu_pos ],LINE2(2));
    }
    return menu_pos;
}

