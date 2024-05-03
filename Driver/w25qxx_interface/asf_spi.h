#ifndef ASF_SPI_H
#define ASF_SPI_H

#include <atmel_start.h>
#include "hpl_spi.h"
#include "asf_w25qxx_custom_descriptor.h"

#ifdef __cplusplus
 extern "C" {
#endif

/**
 * @defgroup spi spi function
 * @brief    spi function modules
 * @{
 */

/**
 * @brief     spi bus init
 * @param[in] *descr custom descriptor
 * @param[in] mode is the spi_transfer_mode
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      SCLK is PB09, MOSI is PB08 MISO is PB14 and CS is PB07
 */
uint8_t spi_init(void *descr, enum spi_transfer_mode mode);

/**
 * @brief  spi bus deinit
 * @param[in] *descr custom descriptor
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t spi_deinit(void *descr);

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
uint8_t spi_write_read(void *descr, uint8_t *in_buf, uint32_t in_len, uint8_t *out_buf, uint32_t out_len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

#if 0
/**
 * @brief     spi bus write command
 * @param[in] descr custom descriptor
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write_cmd(void *descr, uint8_t *buf, uint16_t len);

/**
 * @brief     spi bus write
 * @param[in] descr custom descriptor
 * @param[in] addr is the spi register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write(void *descr, uint8_t addr, uint8_t *buf, uint16_t len);

/**
 * @brief     spi bus write address 16
 * @param[in] descr custom descriptor
 * @param[in] addr is the spi register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t spi_write_address16(void *descr, uint16_t addr, uint8_t *buf, uint16_t len);

/**
 * @brief      spi bus read command
 * @param[in] descr custom descriptor
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read_cmd(void *descr, uint8_t *buf, uint16_t len);

/**
 * @brief      spi bus read
 * @param[in] descr custom descriptor
 * @param[in]  addr is the spi register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read(void *descr, uint8_t addr, uint8_t *buf, uint16_t len);

/**
 * @brief      spi bus read address 16
 * @param[in] descr custom descriptor
 * @param[in]  addr is the spi register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t spi_read_address16(void *descr, uint16_t addr, uint8_t *buf, uint16_t len);

/**
 * @brief      spi transmit
 * @param[in] descr custom descriptor
 * @param[in]  *tx points to a tx buffer
 * @param[out] *rx points to a rx buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 transmit failed
 * @note       none
 */
uint8_t spi_transmit(void *descr, uint8_t *tx, uint8_t *rx, uint16_t len);

#endif
