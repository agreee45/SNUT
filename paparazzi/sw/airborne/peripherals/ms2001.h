/*
 * $Id$
 *
 * Copyright (C) 2008-2009 Antoine Drouin <poinix@gmail.com>
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
 */

#ifndef MS2001_H
#define MS2001_H


#include "std.h"
#define MS2001_NB_AXIS 3

extern void ms2001_init( void );
extern void ms2001_read( void );
extern void ms2001_reset( void);

#define MS2001_IDLE            0
#define MS2001_BUSY            1
#define MS2001_SENDING_REQ     2
#define MS2001_WAITING_EOC     3
#define MS2001_GOT_EOC         4
#define MS2001_READING_RES     5
#define MS2001_DATA_AVAILABLE  6

extern volatile uint8_t ms2001_status;
extern volatile int16_t ms2001_values[MS2001_NB_AXIS];

/* underlying architecture */
#include "peripherals/ms2001_arch.h"
/* must be implemented by underlying architecture */
extern void ms2001_arch_init( void );

#define MS2001_DIVISOR_128  2
#define MS2001_DIVISOR_256  3
#define MS2001_DIVISOR_512  4
#define MS2001_DIVISOR_1024 5

#define MS2001_DIVISOR MS2001_DIVISOR_1024


#endif /* MS2001_H */
