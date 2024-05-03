/*
 * w25qxx_startup_cfg.h
 *
 * Created: may 2024
 * Author: PI
 */ 


#ifndef W25QXX_STARTUP_CFG_H_
#define W25QXX_STARTUP_CFG_H_


#include "driver_w25qxx.h"
#include "driver_w25qxx_basic.h"
#include "driver_w25qxx_advance.h"

#include <atmel_start.h>


#define W25QXX_STARTUP_CFG_TYPE                         W25Q128
#define W25QXX_STARTUP_CFG_INTERFACE                    W25QXX_INTERFACE_SPI
#define W25QXX_STARTUP_CFG_DUAL_QUAD_SPI                W25QXX_BOOL_FALSE
#define W25QXX_STARTUP_CFG_PHY_SPI_INSTANCE_PTR         &SPI_0
#define W25QXX_STARTUP_CFG_PHY_CS_PIN                   SPI_0_CS
#define W25QXX_STARTUP_CFG_PHY_USART_INSTANCE_PTR       &USART_0


extern W25qxx_ASF_CustomDescriptor_s extraDescriptor;

#endif /* W25QXX_STARTUP_CFG_H_ */