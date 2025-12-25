/* 
 * File:   external_eeprom.h
 * Author: hinee
 *
 * Created on 25 September, 2025, 12:32 PM
 */
#ifndef external
#define external

#define SLAVE_READ_E		0xA1
#define SLAVE_WRITE_E		0xA0


void write_external_eeprom(unsigned char address1,  unsigned char data);
unsigned char read_external_eeprom(unsigned char address1);
void ext_eeprom_store_str(unsigned char address,char*str);

#endif

