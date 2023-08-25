#ifndef _AT24C02PZ_H
#define _AT24C02PZ_H

unsigned char At24c02Read(unsigned char addr);
void At24c02Write(unsigned char addr,unsigned char dat);
void EEPROM_WRITE(unsigned char addr_start,unsigned char * dat,unsigned char addr_len);
void EEPROM_READ(unsigned char addr_start,unsigned char * dat,unsigned char addr_len);
#endif