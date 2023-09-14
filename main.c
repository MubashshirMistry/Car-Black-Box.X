/*
 * File:   main.c
 * Author: MUBASHSHIR
 *
 * Created on 7 August, 2023, 6:37 PM
 */


#include "main.h" 

#pragma config WDTE = OFF //Watchdog timer disabled

//unsigned char key_timer;
unsigned char key,pre_key,*view[11],vl;
unsigned long int baud = 9600;
char addr = 0x05,address;//,*view[11];
char memory_location = 0x05;
unsigned char control_flag = DASHBOARD;
char pos1 = -1;
unsigned char chng_pwd,re_chng_pwd,clear_lg;
unsigned char dwnld_lg;


static char view_pos = 0;
static void init_config(void) {
    //Write your initialization code here
    init_clcd();
    init_i2c(100000);
    init_ds1307();
    init_digital_keypad();
    init_adc();
    init_timer2();
    init_uarts(9600);
    
    GIE = 1;
    PEIE = 1;
}

void main(void) {
    init_config(); //Calling initializing function
    unsigned char reset_flag,menu_p;
    char time_set;
    unsigned char dwnld_log;
    
    char *gear[] = {"GN","G1","G2","G3","G4","GR"};
    char event[] = "ON";
   
    //,long_key;
    //char return_time = 5;
    char speed = 0,gr = 0;
    eeprom_at24c04_str_write(0x00,"4455");
    log_car_event(event,speed);
    while (1) {
        //Write application code here
        speed = read_adc() / 10;
        if(speed > 99)
        {
            speed = 99;
        }
        
        key = read_digital_keypad(STATE);
        //long_key = read_digital_keypad(LEVEL);
        for(int j = 3000; j-- ;);
        
        if(key != ALL_RELEASED)
        {
            pre_key = key;
        }
        if(key == SW1)
        {
            strcpy(event,"C ");
             log_car_event(event,speed);
             
        }
        else if( key == SW2 && gr < 6 && control_flag == DASHBOARD)
        {
            strcpy(event,gear[gr]);
            gr++;
             log_car_event(event,speed);
            
        }
        else if(key == SW3 && gr > 0 && control_flag == DASHBOARD)
        {
            gr--;
            strcpy(event,gear[gr]);
             log_car_event(event,speed);
        }
        else if(((key == SW4) || (key == SW5)) && control_flag == DASHBOARD)
        {
            clear_screen();
            clcd_print(" ENTER PASSWORD",LINE1(0));
            clcd_write(CURSOR_POS,INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
            __delay_us(100);
            control_flag = LOGIN_FLAG;
            reset_flag = RESET_PASSWORD;
            TMR2ON = 1;
        }
       
        else if (key == LPSW4 && control_flag == LOGIN_MENU_FLAG) 
        {
                          
                switch(menu_p)
                {
                    case 0:
                      
                        control_flag = VIEW_LOG;
                        reset_flag = RESET_VIEW_LOG_FLAG;
                        clear_screen();                        
                                                                                                                                                                                                                                                                                                                            
                        break;;
                    case 1:
                        log_car_event("CL",speed);
                        control_flag = CLEAR_LOG;
                        clear_screen();
                        clcd_print("CLEAR",LINE1(0));
                        clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                        
                        break;;
                        
                        case 2:
                            clear_screen();
                        control_flag = DOWNLOAD_LOG;
                        reset_flag = RESET_DOWNLOAD_LOG;
                        
                        
                        clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                        
                        break;;
                        
                        case 3:
                        
                        clear_screen();
                        control_flag = CHANGE_PWD_LOG;
                        reset_flag = RESET_NEW_PWD_FLAG;
                        TMR2ON = 1;
                        //clcd_write(CURSOR_POS,INST_MODE);
                        //clcd_write(DISP_ON_AND_CURSOR_ON,INST_MODE);
                      // __delay_us(100);
                        
                       
                        //reset_flag = RESET_PASSWORD;
                       // TMR2ON = 1;
                        
                        break;;
                        
                        case 4:
                        control_flag = SET_TIME_LOG;
                        clear_screen();
                       // control_flag = SET_TIME_FLAG;
                        clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                        
                        break;;
                }
        }
        else if(key == LPSW5 && control_flag == VIEW_LOG)
        {
            
            clear_screen();
            control_flag = LOGIN_MENU_FLAG;
            
            //return LOGIN_MENU_FLAG;
        }
        else if(key == LPSW5 && control_flag == CLEAR_LOG)
        {
            clear_screen();
            control_flag = LOGIN_MENU_FLAG;
            //return LOGIN_MENU_FLAG;
        }
        else if(key == LPSW5 && control_flag == DOWNLOAD_LOG)
        {
            clear_screen();
            control_flag = LOGIN_MENU_FLAG;
            //return LOGIN_MENU_FLAG;
        }
        else if(key == LPSW5 && control_flag == SET_TIME_LOG)
        {
            clear_screen();
            control_flag = LOGIN_MENU_FLAG;
            //return LOGIN_MENU_FLAG;
        }
        else if(key == LPSW5 && control_flag == CHANGE_PWD_LOG)
        {
            clear_screen();
            control_flag = LOGIN_MENU_FLAG;
             clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
            //return LOGIN_MENU_FLAG;
        }
        else if(key == LPSW5 && control_flag == LOGIN_MENU_FLAG)
        {
            clear_screen();
            control_flag = DASHBOARD;
            reset_flag = RESET_PASSWORD;
            
                    
            //return LOGIN_MENU_FLAG;
        }
            
               
               
       
        switch(control_flag)
        {
            case DASHBOARD: 
                display_dashboard(event,speed);//DASHBOARD
                break;;
                
            case LOGIN_FLAG:
                switch(login(key,reset_flag))
                {
                    case RETURN_BACK:
                        control_flag = DASHBOARD;
                        TMR2ON = 0;
                        clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                        __delay_us(100);
                        break;;
                    case LOGIN_SUCCESS:
                        control_flag = LOGIN_MENU_FLAG;
                        reset_flag = RESET_LOGIN_MENU;
                        TMR2ON = 0;
                        clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                        __delay_us(100);
                        continue;
                        break;
                } 
                break;;
            case LOGIN_MENU_FLAG:
                menu_p = (login_menu(key,reset_flag));
                break;;
                
            case VIEW_LOG:
               //clear_screen();
                 vl = view_log(key,reset_flag,control_flag);
                 if(vl == FAILURE)
                 {
                     clear_screen;
                      clcd_print("NO LOGS AVAILABLE", LINE1(0));
        __delay_ms(2000);
clear_screen();
                 control_flag = LOGIN_MENU_FLAG;       
                 }
                 
                 break;;
                 
            case CHANGE_PWD_LOG:
                
                 chng_pwd = change_pwd(key,reset_flag,control_flag);
                 
                 if(chng_pwd == PWD_CHANGED)
                 {
                     clear_screen();
                      TMR2ON = 0;
                        
                     clcd_print("PWD CHANGED",LINE1(2));
                     __delay_ms(3000);
                     clear_screen();
                     control_flag = LOGIN_MENU_FLAG;
                     reset_flag = RESET_NEW_PWD_FLAG;
                     clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                 }
                 else if(chng_pwd == PWD_NOT_CHANGED)
                 {
                     clear_screen();
                      TMR2ON = 0;
                        clcd_write(DISP_ON_AND_CURSOR_OFF,INST_MODE);
                     clcd_print("PWD NOT CHANGED",LINE1(2));
                     clcd_print("TRY AGAIN",LINE2(3));
                     __delay_ms(3000);
                     clear_screen();
                     control_flag = LOGIN_MENU_FLAG;
                     reset_flag = RESET_NEW_PWD_FLAG;
                     
                 }
                 break;;
            
            case CLEAR_LOG:
                clear_screen();
                reset_flag = RESET_CLEAR_LOG;
               clear_lg = clear_log(reset_flag);
               if(clear_lg == LOGS_CLEARED)
               {
                    clcd_print("LOGS CLEARED",LINE1(3));
                    clcd_print("SUCCESSFULLY",LINE2(3));
                    __delay_ms(500);
                    clear_screen();
                    control_flag = LOGIN_MENU_FLAG;
               }
               
               break;;
               
            case SET_TIME_LOG:
                
               
                time_set = set_time(key,control_flag);
                if(time_set == SUCCESS)
                {
                   
        clcd_print("  TIME CHANGED  ",LINE1(0));
        clcd_print("  SUCCESSFULLY  ",LINE2(0));
        __delay_ms(3000);
        clear_screen();
        control_flag = LOGIN_MENU_FLAG;
                }
                
                break;;
                
            case DOWNLOAD_LOG:
                
                
             
                    download_log();
               
                clear_screen();
                control_flag = LOGIN_MENU_FLAG;
                
                break;;
                
                
        }
        reset_flag = RESET_NOTHING;
    }

}

