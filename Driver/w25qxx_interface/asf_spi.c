#include "asf_spi.h"

/**
 * @brief spi var definition
 */

/**
 * @brief  spi cs init
 * @param[in] *descr custom descriptor
 * @return status code
 *         - 0 success
 * @note   none
 */
static uint8_t a_spi_cs_init(void *descr)
{

#if 0
    GPIO_InitTypeDef GPIO_InitStruct;

    /* enable cs gpio clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* gpio init */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
#endif

    return 0;

}

/**
 * @brief     spi bus init
 * @param[in] *descr custom descriptor
 * @param[in] mode is the spi mode
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note     
 */
uint8_t spi_init(void *descr, enum spi_transfer_mode mode)
{
#if 0
    g_spi_handle.Instance = SPI1;
    g_spi_handle.Init.Mode = SPI_MODE_MASTER;
    g_spi_handle.Init.Direction = SPI_DIRECTION_2LINES;
    g_spi_handle.Init.DataSize = SPI_DATASIZE_8BIT;

    /* set the mode */
    if (mode == SPI_MODE_0)
    {
        g_spi_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
        g_spi_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
    }
    else if (mode == SPI_MODE_1)
    {
        g_spi_handle.Init.CLKPolarity = SPI_POLARITY_LOW;
        g_spi_handle.Init.CLKPhase = SPI_PHASE_2EDGE;
    }
    else if (mode == SPI_MODE_2)
    {
        g_spi_handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
        g_spi_handle.Init.CLKPhase = SPI_PHASE_1EDGE;
    }
    else
    {
        g_spi_handle.Init.CLKPolarity = SPI_POLARITY_HIGH;
        g_spi_handle.Init.CLKPhase = SPI_PHASE_2EDGE;
    }
    g_spi_handle.Init.NSS = SPI_NSS_SOFT;
    g_spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    g_spi_handle.Init.FirstBit = SPI_FIRSTBIT_MSB;
    g_spi_handle.Init.TIMode = SPI_TIMODE_DISABLE;
    g_spi_handle.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    g_spi_handle.Init.CRCPolynomial = 10;

    /* spi init */
    if (HAL_SPI_Init(&g_spi_handle) != HAL_OK)
    {
        return 1;
    }
#endif

    return a_spi_cs_init(descr);
}

/**
 * @brief  spi bus deinit
 * @param[in] *descr custom descriptor
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t spi_deinit(void *descr)
{
// TODO: ...
    descr = NULL;
#if 0
    /* cs deinit */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);

    /* spi deinit */
    if (HAL_SPI_DeInit(&g_spi_handle) != HAL_OK)
    {
        return 1;
    }
#endif

    return 0;
}


/**
 * @brief      spi bus write read
 * @param[in]  *descr custom descriptor
 * @param[in]  *in_buf points to an input buffer
 * @param[in]  in_len is the input length
 * @param[out] *out_buf points to an output buffer
 * @param[in]  out_len is the output length
 * @return     status code
 *             - 0 success
 *             - 1 write read failed
 * @note       none
 */
uint8_t spi_write_read(void *descr, uint8_t *in_buf, uint32_t in_len, uint8_t *out_buf, uint32_t out_len)
{
    if (NULL == descr)
    {
        return -1;
    }

    /* extract SPI instance and CS pin */
    W25qxx_ASF_CustomDescriptor_s extraDescriptor = *(W25qxx_ASF_CustomDescriptor_s*)descr;
    struct spi_m_sync_descriptor spiInstance = *extraDescriptor.spiInstance;        //  = SPI_0
    const uint8_t chipSelectPin = extraDescriptor.chipSelectPin;                    // = SPI_0_CS
    //struct usart_async_descriptor usartInstance = *extraDescriptor.usartInstance; // = USART_0 not used here

    /* set cs low */
    gpio_set_pin_level(chipSelectPin, false);

    if (in_len > 0)
    {
        /* transmit the input buffer */
        struct io_descriptor *io;
        spi_m_sync_get_io_descriptor(&spiInstance, &io);
        spi_m_sync_enable(&spiInstance);
        io_write(io, in_buf, in_len);
    }

    if (out_len > 0)
    {
        /* transmit to the output buffer */
        struct io_descriptor *io;
        spi_m_sync_get_io_descriptor(&spiInstance, &io);
        spi_m_sync_enable(&spiInstance);
        io_read(io, out_buf, out_len);
    }

    /* set cs high */
    gpio_set_pin_level(chipSelectPin, true);

    return 0;
}



#if 0
/**
 * @brief     spi bus write command
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write_cmd(uint8_t *buf, uint16_t len)
{
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    /* if len > 0 */
    if (len > 0)
    {
        /* transmit the buffer */
        res = HAL_SPI_Transmit(&g_spi_handle, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief     spi bus write
 * @param[in] addr is the spi register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write(uint8_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer;
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    /* transmit the addr */
    buffer = addr;
    res = HAL_SPI_Transmit(&g_spi_handle, (uint8_t *)&buffer, 1, 1000);
    if (res != HAL_OK)
    {
        /* set cs high */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

        return 1;
    }

    /* if len > 0 */
    if (len > 0)
    {
        /* transmit the buffer */
        res = HAL_SPI_Transmit(&g_spi_handle, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief     spi bus write address 16
 * @param[in] addr is the spi register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write_address16(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer[2];
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    /* transmit the addr  */
    buffer[0] = (addr >> 8) & 0xFF;
    buffer[1] = addr & 0xFF;
    res = HAL_SPI_Transmit(&g_spi_handle, (uint8_t *)buffer, 2, 1000);
    if (res != HAL_OK)
    {
        /* set cs high */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

        return 1;
    }

    /* if len > 0 */
    if (len > 0)
    {
        /* transmit the buffer */
        res = HAL_SPI_Transmit(&g_spi_handle, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief      spi bus read command
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read_cmd(uint8_t *buf, uint16_t len)
{
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    /* if len > 0 */
    if (len > 0)
    {
        /* receive to the buffer */
        res = HAL_SPI_Receive(&g_spi_handle, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief      spi bus read
 * @param[in]  addr is the spi register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read(uint8_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer;
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    /* transmit the addr */
    buffer = addr;
    res = HAL_SPI_Transmit(&g_spi_handle, (uint8_t *)&buffer, 1, 1000);
    if (res != HAL_OK)
    {
        /* set cs high */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

        return 1;
    }

    /* if len > 0 */
    if (len > 0)
    {
        /* receive to the buffer */
        res = HAL_SPI_Receive(&g_spi_handle, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief      spi bus read address 16
 * @param[in]  addr is the spi register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read_address16(uint16_t addr, uint8_t *buf, uint16_t len)
{
    uint8_t buffer[2];
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    /* transmit the addr  */
    buffer[0] = (addr >> 8) & 0xFF;
    buffer[1] = addr & 0xFF;
    res = HAL_SPI_Transmit(&g_spi_handle, (uint8_t *)buffer, 2, 1000);
    if (res != HAL_OK)
    {
        /* set cs high */
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

        return 1;
    }

    /* if len > 0 */
    if (len > 0)
    {
        /* receive to the buffer */
        res = HAL_SPI_Receive(&g_spi_handle, buf, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    return 0;
}

/**
 * @brief      spi transmit
 * @param[in]  *tx points to a tx buffer
 * @param[out] *rx points to a rx buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 transmit failed
 * @note       none
 */
uint8_t spi_transmit(uint8_t *tx, uint8_t *rx, uint16_t len)
{
    uint8_t res;

    /* set cs low */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);

    /* if len > 0 */
    if (len > 0)
    {
        /* transmit */
        res = HAL_SPI_TransmitReceive(&g_spi_handle, tx, rx, len, 1000);
        if (res != HAL_OK)
        {
            /* set cs high */
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

            return 1;
        }
    }

    /* set cs high */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);

    return 0;
}
#endif
