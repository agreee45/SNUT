/*
 * Paparazzi autopilot $Id: gps_ubx.h 4659 2010-03-10 16:55:04Z mmm $
 *
 * Copyright (C) 2004-2006  Pascal Brisset, Antoine Drouin
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

/** \file gps_xsens.h
 * \brief XSens GPS
 *
*/


#ifndef XSENS_GPS_H
#define XSENS_GPS_H

extern uint16_t gps_reset;

#define GPS_NB_CHANNELS 16

extern void reset_gps_watchdog(void);


#define GpsFixValid() (gps_mode > 0)

#define gps_xsens_Reset(_val) { \
  gps_reset = _val; \
}


#endif
