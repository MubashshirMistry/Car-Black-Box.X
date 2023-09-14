/* 
 * File:   uart.h
 */

#ifndef UART_H
#define	UART_H

#define FOSC                20000000

void init_uarts(unsigned long baud);
unsigned char getcharr(void);
void putcharr(unsigned char data);
void putss(const char *s);

#endif	/* UART_H */