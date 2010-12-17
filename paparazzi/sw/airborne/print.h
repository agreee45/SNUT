/*
 * Paparazzi $I$
 *
 * Copyright (C) 2005 Pascal Brisset, Antoine Drouin
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

#ifndef PRINT_H
#define PRINT_H

#include "mcu_periph/uart.h"
#include "mcu_periph/usb_serial.h"

#define _PrintString(out_fun, s) { \
  uint8_t i = 0;                  \
  while (s[i]) {                  \
    out_fun(s[i]);		  \
    i++;                          \
  }                               \
}

#define _PrintHex(out_fun, c) {						\
    const uint8_t hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7',   \
			      '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' }; \
    uint8_t high = (c & 0xF0)>>4;					\
    uint8_t low  = c & 0x0F;						\
    out_fun(hex[high]);							\
    out_fun(hex[low]);							\
}									\

#define _PrintHex16(out_fun, c ) {			\
    uint8_t high16 = (uint8_t)(c>>8);			\
    uint8_t low16  = (uint8_t)(c);			\
    _PrintHex(out_fun, high16);				\
    _PrintHex(out_fun, low16);				\
}

#define _PrintHex32(out_fun, c ) {			\
    uint16_t high32 = (uint16_t)(c>>16);			\
    uint16_t low32  = (uint16_t)(c);			\
    _PrintHex16(out_fun, high32);				\
    _PrintHex16(out_fun, low32);				\
}

#define Uart0PrintHex(c) _PrintHex(uart0_transmit, c)
#define Uart0PrintHex16(c) _PrintHex16(uart0_transmit, c)
#define Uart0PrintHex32(c) _PrintHex32(uart0_transmit, c)
#define Uart0PrintString(s) _PrintString(uart0_transmit, s)

#define Uart1PrintHex(c) _PrintHex(uart1_transmit, c)
#define Uart1PrintHex16(c) _PrintHex16(uart1_transmit, c)
#define Uart1PrintString(s) _PrintString(uart1_transmit, s)

#define UsbSPrintHex(c) _PrintHex(VCOM_putchar, c)
#define UsbSPrintHex16(c) _PrintHex16(VCOM_putchar, c)
#define UsbSPrintString(s) _PrintString(VCOM_putchar, s)


#endif /* PRINT_H */

