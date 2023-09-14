/* 
 * File:   at24c04.h
 * Author: MUBASHSHIR
 *
 * Created on 9 August, 2023, 6:25 PM
 */

#ifndef AT24C04_H
#define	AT24C04_H

#define SLAVE_READ_EE   0XA1
#define SLAVE_WRITE_EE  0xA0

unsigned char eeprom_at24c04_randon_read(unsigned char memory_loc);
void eeprom_at24c04_byte_write(unsigned char memory_loc, unsigned char data);
void eeprom_at24c04_str_write(unsigned char memory_loc, char *data);

#endif	/* AT24C04_H */

