/* 
 * File:   cbb.h
 * Author: MUBASHSHIR
 *
 * Created on 7 August, 2023, 8:45 PM
 */

#ifndef CBB_H
#define	CBB_H

void display_dashboard(char *event, char speed);
void log_car_event(char *event, char speed);
void log_event(void);
void clear_screen(void);
unsigned char login(unsigned char key,unsigned char reset_flag);
unsigned char login_menu(unsigned char key, unsigned char reset_flag);
unsigned char view_log(unsigned char button,unsigned char reset_flag,unsigned char control_flag);
unsigned char change_pwd(unsigned char key,unsigned char reset_flag,unsigned char control_flag);
unsigned char clear_log(unsigned char reset_flag);
unsigned char view_log_scroll(unsigned char key,unsigned char addr,unsigned char control_flag);
unsigned char set_time(unsigned char key,unsigned char control_flag);
void download_log(void);

#endif	/* CBB_H */

