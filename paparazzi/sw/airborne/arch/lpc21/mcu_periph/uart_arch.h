/*
 * $Id$
 *
 * Copyright (C) 2008  Pascal Brisset, Antoine Drouin
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#ifndef LPC21_UART_ARCH_H
#define LPC21_UART_ARCH_H

#include "types.h"
#include "LPC21xx.h"
#include BOARD_CONFIG

#define UART0_RX_BUFFER_SIZE 128        // UART0 receive buffer size
#define UART0_TX_BUFFER_SIZE 128        // UART0 transmit buffer size
#define UART1_RX_BUFFER_SIZE 128        // UART1 receive buffer size
#define UART1_TX_BUFFER_SIZE 128        // UART1 transmit buffer size

#define UART_BAUD(baud) (uint16_t)((PCLK / ((baud) * 16.0)) + 0.5)

#define B1200         UART_BAUD(1200)
#define B2400         UART_BAUD(2400)
#define B9600         UART_BAUD(9600)
#define B19200        UART_BAUD(19200)
#define B38400        UART_BAUD(38400)
#define B57600        UART_BAUD(57600)
#define B115200       UART_BAUD(115200)
#define B230400       UART_BAUD(230400)

#define UART_8N1      (uint8_t)(ULCR_CHAR_8 + ULCR_PAR_NO   + ULCR_STOP_1)
#define UART_7N1      (uint8_t)(ULCR_CHAR_7 + ULCR_PAR_NO   + ULCR_STOP_1)
#define UART_8N2      (uint8_t)(ULCR_CHAR_8 + ULCR_PAR_NO   + ULCR_STOP_2)
#define UART_7N2      (uint8_t)(ULCR_CHAR_7 + ULCR_PAR_NO   + ULCR_STOP_2)
#define UART_8E1      (uint8_t)(ULCR_CHAR_8 + ULCR_PAR_EVEN + ULCR_STOP_1)
#define UART_7E1      (uint8_t)(ULCR_CHAR_7 + ULCR_PAR_EVEN + ULCR_STOP_1)
#define UART_8E2      (uint8_t)(ULCR_CHAR_8 + ULCR_PAR_EVEN + ULCR_STOP_2)
#define UART_7E2      (uint8_t)(ULCR_CHAR_7 + ULCR_PAR_EVEN + ULCR_STOP_2)
#define UART_8O1      (uint8_t)(ULCR_CHAR_8 + ULCR_PAR_ODD  + ULCR_STOP_1)
#define UART_7O1      (uint8_t)(ULCR_CHAR_7 + ULCR_PAR_ODD  + ULCR_STOP_1)
#define UART_8O2      (uint8_t)(ULCR_CHAR_8 + ULCR_PAR_ODD  + ULCR_STOP_2)
#define UART_7O2      (uint8_t)(ULCR_CHAR_7 + ULCR_PAR_ODD  + ULCR_STOP_2)

#define UART_FIFO_OFF (0x00)
#define UART_FIFO_1   (uint8_t)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG1)
#define UART_FIFO_4   (uint8_t)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG4)
#define UART_FIFO_8   (uint8_t)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG8)
#define UART_FIFO_14  (uint8_t)(UFCR_FIFO_ENABLE + UFCR_FIFO_TRIG14)


extern uint16_t uart0_rx_insert_idx, uart0_rx_extract_idx;
extern uint8_t uart0_rx_buffer[UART0_RX_BUFFER_SIZE];

#define Uart0ChAvailable() (uart0_rx_insert_idx != uart0_rx_extract_idx)

#define Uart0Getch() ({\
   uint8_t ret = uart0_rx_buffer[uart0_rx_extract_idx]; \
   uart0_rx_extract_idx = (uart0_rx_extract_idx + 1)%UART0_RX_BUFFER_SIZE;        \
   ret;                                                 \
})


extern uint16_t uart1_rx_insert_idx, uart1_rx_extract_idx;
extern uint8_t uart1_rx_buffer[UART1_RX_BUFFER_SIZE];
extern void uart1_init_param( uint16_t baud, uint8_t mode, uint8_t fmode);
extern void uart0_init_param( uint16_t baud, uint8_t mode, uint8_t fmode);

#define Uart1ChAvailable() (uart1_rx_insert_idx != uart1_rx_extract_idx)

#define Uart1Getch() ({\
   uint8_t ret = uart1_rx_buffer[uart1_rx_extract_idx]; \
   uart1_rx_extract_idx = (uart1_rx_extract_idx + 1)%UART1_RX_BUFFER_SIZE;        \
   ret;                                                 \
})

extern uint8_t uart0_tx_running;
extern uint8_t uart1_tx_running;

#endif /* LPC21_UART_ARCH_H */
