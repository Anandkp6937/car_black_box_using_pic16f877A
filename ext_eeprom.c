/* 
 * File:   external_eeprom.c
 * Author: hinee
 *
 * Created on 25 September, 2025, 12:32 PM
 */
#include <xc.h>
#include "ext_eeprom.h"
#include "i2c.h"
void write_external_eeprom(unsigned char address, unsigned char data)
{
    i2c_start();
    i2c_write(SLAVE_WRITE_E);
    i2c_write(address);
    i2c_write(data);
    i2c_stop();
    for (unsigned int wait = 3000;wait--;);
}

unsigned char read_external_eeprom(unsigned char address)
{
    unsigned char data;

    i2c_start();
    i2c_write(SLAVE_WRITE_E);
    i2c_write(address);
    i2c_rep_start();
    i2c_write(SLAVE_READ_E);
    data = i2c_read(0);
    i2c_stop();

    return data;
}

void ext_eeprom_store_str(unsigned char address,char*str){

    while(*str!='\0'){
     write_external_eeprom(address,*str);
     str++;
     address++;
    }


}
