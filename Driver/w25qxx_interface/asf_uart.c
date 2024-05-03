#include "asf_uart.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


/**
 * @brief uart var definition
 */
extern struct usart_async_descriptor USART_0;   /**< uart handle */
// TODO: extract usart_async_descriptor from void *extra

uint8_t g_uart_rx_buffer[UART_MAX_LEN];        /**< uart rx buffer */
uint8_t g_uart_buffer;                         /**< uart one buffer */
volatile uint16_t g_uart_point;                /**< uart rx point */
volatile uint8_t g_uart_tx_done;               /**< uart tx done flag */

static struct io_descriptor *io;

/**
 * @brief     uart init with 8 data bits, 1 stop bit and no parity
 * @param[in] baud is the baud rate
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      TX is PA9 and RX is PA10
 */
uint8_t uart_init(uint32_t baud)
{

//TODO: set baud-rate!

    usart_async_register_callback(&USART_0, USART_ASYNC_TXC_CB, (usart_cb_t)uart_set_tx_done);
    usart_async_register_callback(&USART_0, USART_ASYNC_RXC_CB, (usart_cb_t)uart_irq_handler);
    usart_async_register_callback(&USART_0, USART_ASYNC_ERROR_CB, (usart_cb_t)uart_err_cb);
    usart_async_get_io_descriptor(&USART_0, &io);
    usart_async_enable(&USART_0);

#if 0
    g_uart_handle.Instance = USART1;
    g_uart_handle.Init.BaudRate = baud;
    g_uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
    g_uart_handle.Init.StopBits = UART_STOPBITS_1;
    g_uart_handle.Init.Parity = UART_PARITY_NONE;
    g_uart_handle.Init.Mode = UART_MODE_TX_RX;
    g_uart_handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    g_uart_handle.Init.OverSampling = UART_OVERSAMPLING_16;

    /* uart init */
    if (HAL_UART_Init(&g_uart_handle) != HAL_OK)
    {
        return 1;
    }

    /* receive one byte */
    if (HAL_UART_Receive_IT(&g_uart_handle, (uint8_t *)&g_uart_buffer, 1) != HAL_OK)
    {
        return 1;
    }
#endif

    return 0;
}

/**
 * @brief  uart deint
 * @return status code
 *         - 0 success
 *         - 1 deinit
 * @note   none
 */
uint8_t uart_deinit(void)
{
    usart_async_disable(&USART_0);

    return 0;
}

/**
 * @brief     uart write data
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t uart_write(uint8_t *buf, uint16_t len)
{
    uint16_t timeout = 1000;

    /* set tx done 0 */
    g_uart_tx_done = 0;


    io_write(io, (uint8_t *)buf, len);


    /* wait for events */
    while ((g_uart_tx_done == 0) && (timeout != 0))
    {
        delay_ms(1);
        timeout--;
    }

    /* check the timeout */
    if (timeout != 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief      uart read data
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     length of the read data
 * @note       this function will clear all received buffer even read length is less than received length
 */
uint16_t uart_read(uint8_t *buf, uint16_t len)
{
    uint16_t read_len;
    uint16_t g_uart_point_old;

    /* check receiving */
    start:
    g_uart_point_old = g_uart_point;
    delay_ms(1);
    if (g_uart_point > g_uart_point_old)
    {
        goto start;
    }

    /* copy the data */
    read_len = (len < g_uart_point) ? len : g_uart_point;
    memcpy(buf, g_uart_rx_buffer, read_len);

    /* clear the buffer */
    g_uart_point = 0;

    return read_len;
}

/**
 * @brief  uart flush data
 * @return status code
 *         - 0 success
 * @note   none
 */
uint16_t uart_flush(void)
{
    /* clear the buffer */
    g_uart_point = 0;

    return 0;
}

/**
 * @brief     uart print format data
 * @param[in] fmt is the format data
 * @return    length of the sent data
 * @note      none
 */
uint16_t uart_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;

    /* print to the buffer */
    memset((char *)str, 0, sizeof(char) * 256);
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);

    /* send the data */
    len = strlen((char *)str);
    if (uart_write((uint8_t *)str, len) != 0)
    {
        return 0;
    }
    else
    {
        return len;
    }
}

#if 0
/**
 * @brief  uart get the handle
 * @return points to a uart handle
 * @note   none
 */
UART_HandleTypeDef* uart_get_handle(void)
{
    return &g_uart_handle;
}
#endif

/**
 * @brief uart set tx done
 * @note  none
 */
void uart_set_tx_done(const struct usart_async_descriptor *const descr)
{
    g_uart_tx_done = 1;
}


/**
 * @brief uart error callback
 * @note  none
 */
void uart_err_cb(const struct usart_async_descriptor *const descr)
{
    while(1)
        ;
}


/**
 * @brief uart irq handler
 * @note  none
 */
void uart_irq_handler(const struct usart_async_descriptor *const descr)
{

    /* receive one byte */
    io_read(io, (uint8_t *)&g_uart_buffer, 1);

    /* save one byte */
    g_uart_rx_buffer[g_uart_point] = g_uart_buffer;
    g_uart_point++;
    if (g_uart_point > (UART_MAX_LEN - 1))
    {
        g_uart_point = 0;
    }

}
