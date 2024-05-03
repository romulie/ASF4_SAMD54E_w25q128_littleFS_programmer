#ifndef ASF_W25QXX_CUSTOM_DESCRIPTOR_H
#define ASF_W25QXX_CUSTOM_DESCRIPTOR_H


#include <atmel_start.h>


typedef struct customDescriptor_s
{
    struct spi_m_sync_descriptor *spiInstance;      //  = &SPI_0
    uint8_t chipSelectPin;                          // = SPI_0_CS
    struct usart_async_descriptor *usartInstance;   // = &USART_0

} W25qxx_ASF_CustomDescriptor_s;




#endif