#ifndef MAIN_H
#define MAIN_H

#include<string.h>
#include "adc.h"
#include "clcd.h"
#include "bb.h"
#include "i2c.h"
#include "ds1307.h"
#include "digi.h"
#include "tiimer.h"
#include "ext_eeprom.h"
#include "uart.h"

void config(void);

#define HOMESCREEN_FLAG 0x01
#define LOGIN_SCREEN_FLAG 0x02
#define MENU_FLAG 0x03
#define RESET_ 0x00
#define RESET_PW 0x08
#define NO_RESET 0x04
#define RETURN_BACK 0x05
#define SUCCESS 0x07
#define VIEW_LOG_FLAG 0x08
#define CLEAR_LOG_FLAG 0x09
#define CHANGE_PASSWORD_FLAG 0x10
#define SET_TIME_FLAG 0x11
#define DOWNLOAD_FLAG 0x12
#define LOG_START_ADDR   0x00
#define LOG_SIZE         100

#define FIELD_SS 0x13
#define FIELD_MM 0x14
#define FIELD_HH 0x15
extern volatile unsigned char potVal[4];


#endif