#ifndef ASF_UART_H
#define ASF_UART_H

#include <atmel_start.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup uart uart function
 * @brief    uart function modules
 * @{
 */

/**
 * @brief uart max rx buffer length definition
 */
#define UART_MAX_LEN        256        /**< uart max len */

/**
 * @brief     uart init with 8 data bits, 1 stop bit and no parity
 * @param[in] baud is the baud rate
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      TX is PA9 and RX is PA10
 */
uint8_t uart_init(uint32_t baud_rate);

/**
 * @brief  uart deint
 * @return status code
 *         - 0 success
 *         - 1 deinit
 * @note   none
 */
uint8_t uart_deinit(void);

/**
 * @brief     uart write data
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t uart_write(uint8_t *buf, uint16_t len);

/**
 * @brief      uart read data
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     length of the read data
 * @note       this function will clear all received buffer even read length is less than received length
 */
uint16_t uart_read(uint8_t *buf, uint16_t len);

/**
 * @brief  uart flush data
 * @return status code
 *         - 0 success
 * @note   none
 */
uint16_t uart_flush(void);

/**
 * @brief     uart print format data
 * @param[in] fmt is the format data
 * @return    length of the sent data
 * @note      none
 */
uint16_t uart_print(const char *const fmt, ...);

#if 0
/**
 * @brief  uart get the handle
 * @return points to a uart handle
 * @note   none
 */
UART_HandleTypeDef* uart_get_handle(void);
#endif

/**
 * @brief uart set tx done
 * @note  none
 */
void uart_set_tx_done(const struct usart_async_descriptor *const descr);

/**
 * @brief uart irq handlers
 * @note  none
 */
void uart_irq_handler(const struct usart_async_descriptor *const descr);
void uart_err_cb(const struct usart_async_descriptor *const descr);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
