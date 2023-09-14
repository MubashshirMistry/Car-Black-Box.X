/* 
 * File:   main.h
 * Author: MUBASHSHIR
 *
 * Created on 7 August, 2023, 6:53 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
#include "clcd.h"
#include "digital_keypad.h"
#include "ds1307.h"
#include "adc.h"
#include "i2c.h"
#include "cbb.h"
#include<string.h>
#include "at24c04.h"
#include "timers.h"
#include "uart.h"

#define DASHBOARD   0x01
#define LOGIN_FLAG  0x02
#define LOGIN_MENU_FLAG 0x03
#define SUCCESS 1
#define FAILURE 0

#define VIEW_LOG    0x04
#define CLEAR_LOG   0x05
#define DOWNLOAD_LOG    0x06
#define CHANGE_PWD_LOG  0x07
#define SET_TIME_LOG    0x08
#define PWD_CHANGED 0x09
#define PWD_NOT_CHANGED 0x0A


#define RESET_PASSWORD  0x10
#define RESET_NOTHING   0x20
#define RETURN_BACK     0x30
#define LOGIN_SUCCESS   0x40
#define RESET_LOGIN_MENU    0x50
#define RESET_VIEW_LOG_FLAG 0x60
#define RESET_NEW_PWD_FLAG  0x70
#define RESET_NEW_PWD_FLAG2 0x80
#define RESET_CLEAR_LOG 0x90
#define LOGS_CLEARED    0xA0
#define RESET_DOWNLOAD_LOG  0xB0


#define CLEAR_LINE2 clcd_print("                ",LINE2(0));

#include <stdio.h>

void display_view_log(void);





#endif	/* MAIN_H */

