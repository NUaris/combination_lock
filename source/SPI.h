#ifndef _SPI_H
#define _SPI_H

unsigned char SPIReadByte(void);
void SPIWriteByte(unsigned char SPIData);

#define SET_SPI_CS  spi_cs=1
#define CLR_SPI_CS  spi_cs=0

#define SET_SPI_CK  spi_ck=1
#define CLR_SPI_CK  spi_ck=0

#define SET_SPI_MOSI  spi_mosi=1
#define CLR_SPI_MOSI  spi_mosi=0

#define STU_SPI_MISO  spi_miso

#define SET_RC522RST  spi_rst=1
#define CLR_RC522RST  spi_rst=0

#endif