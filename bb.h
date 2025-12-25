#ifndef BB_H
#define BB_H

//fetch pot value 
void getPotVal();

void disp_dashboard(char event[]);

void clear_screen(void);

char login(unsigned char key,unsigned char reset_flag);


char login_menu(unsigned char key);


 char view_log(unsigned char key);
 
 
 char change_passwd(unsigned char key);
 
 char log_event_to_eeprom(unsigned char event [],unsigned char speed []);
 
 void log_car_event(char event[],char speed[]);
 
 void clear_logs(void);
 
 unsigned char dec_to_bcd(unsigned char val);
 
 
 char set_time(unsigned char key);
 
 void clear_eeprom_log(void);
 
 char download_log(void);
#endif