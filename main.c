/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2021-07-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/07/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include <atmel_start.h>

//#include "shell.h"
//#include "getopt.h"

#include "asf_uart.h"
#include "w25qxx_startup.h"
#include "littlefs_startup.h"

//#include <stdio.h>


/**
 * @brief global var definition
 */
uint8_t g_buf[256];                        /**< uart buffer */
volatile uint16_t g_len;                   /**< uart buffer length */


/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

// TODO: Include MSP_registry in project and register all objects

    /* uart init */
    uart_init(115200); // TODO: uart_init() doesn't set baud rate now (it is set-up through Atmel Start)
    uart_print("w25qxx: welcome to w25qxx programmer.\n");

    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {

            // set-up flash chip
            W25qxx_Startup();
            uart_print("w25qxx: flash chip startup() OK!\n");
            // set-up LittleFS here
            Littlefs_Startup();
            uart_print("LittleFS: filesystem startup() OK!\n");

            uart_print("w25qxx: go over again.\n");
            uart_flush();
        }
        delay_ms(100);
        gpio_toggle_pin_level(USER_LED);
    }
}
