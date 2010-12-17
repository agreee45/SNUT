/*
 * $Id$
 *
 * Copyright (C) 2007-2009  ENAC, Pascal Brisset, Antoine Drouin
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

#include "subsystems/navigation/common_nav.h"
#include "estimator.h"
#include "generated/flight_plan.h"
#include "gps.h"

float dist2_to_home;
float dist2_to_wp;

bool_t too_far_from_home;

const uint8_t nb_waypoint = NB_WAYPOINT;
struct point waypoints[NB_WAYPOINT] = WAYPOINTS;

uint8_t nav_stage, nav_block;
uint16_t stage_time, block_time;

/** To save the current block/stage to enable return */
uint8_t last_block, last_stage;

float ground_alt;

int32_t nav_utm_east0 = NAV_UTM_EAST0;
int32_t nav_utm_north0 = NAV_UTM_NORTH0;
uint8_t nav_utm_zone0 = NAV_UTM_ZONE0;
float max_dist_from_home = MAX_DIST_FROM_HOME;

/** \brief Computes square distance to the HOME waypoint potentially sets
 * \a too_far_from_home
 */
void compute_dist2_to_home(void) {
  float ph_x = waypoints[WP_HOME].x - estimator_x;
  float ph_y = waypoints[WP_HOME].y - estimator_y;
  dist2_to_home = ph_x*ph_x + ph_y *ph_y;
  too_far_from_home = dist2_to_home > (MAX_DIST_FROM_HOME*MAX_DIST_FROM_HOME);
#if defined InAirspace
  too_far_from_home = too_far_from_home || !(InAirspace(estimator_x, estimator_y));
#endif
}


static float previous_ground_alt;

/** Reset the geographic reference to the current GPS fix */
unit_t nav_reset_reference( void ) {
#ifdef GPS_USE_LATLONG
  /* Set the real UTM zone */
  nav_utm_zone0 = (gps_lon/10000000+180) / 6 + 1;

  /* Recompute UTM coordinates in this zone */
  latlong_utm_of(RadOfDeg(gps_lat/1e7), RadOfDeg(gps_lon/1e7), nav_utm_zone0);
  nav_utm_east0 = latlong_utm_x;
  nav_utm_north0 = latlong_utm_y;
#else
  nav_utm_zone0 = gps_utm_zone;
  nav_utm_east0 = gps_utm_east/100;
  nav_utm_north0 = gps_utm_north/100;
#endif

  previous_ground_alt = ground_alt;
  ground_alt = gps_alt/100;
  return 0;
}

/** Shift altitude of the waypoint according to a new ground altitude */
unit_t nav_update_waypoints_alt( void ) {
  uint8_t i;
  for(i = 0; i < NB_WAYPOINT; i++) {
    waypoints[i].a += ground_alt - previous_ground_alt;
  }
  return 0;
}

void nav_init_block(void) {
  if (nav_block >= NB_BLOCK)
    nav_block=NB_BLOCK-1;
  nav_stage = 0;
  block_time = 0;
  InitStage();
}

void nav_goto_block(uint8_t b) {
  if (b != nav_block) { /* To avoid a loop in a the current block */
    last_block = nav_block;
    last_stage = nav_stage;
  }
  GotoBlock(b);
}

void common_nav_periodic_task_4Hz() {
  RunOnceEvery(4, { stage_time++;  block_time++; });
}

void nav_move_waypoint(uint8_t wp_id, float ux, float uy, float alt) {
  if (wp_id < nb_waypoint) {
    float dx, dy;
    dx = ux - nav_utm_east0 - waypoints[WP_HOME].x;
    dy = uy - nav_utm_north0 - waypoints[WP_HOME].y;
    BoundAbs(dx, max_dist_from_home);
    BoundAbs(dy, max_dist_from_home);
    waypoints[wp_id].x = waypoints[WP_HOME].x + dx;
    waypoints[wp_id].y = waypoints[WP_HOME].y + dy;
    waypoints[wp_id].a = alt;
  }
}
