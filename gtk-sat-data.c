/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
  Gpredict: Real-time satellite tracking and orbit prediction program

  Copyright (C)  2001-2009  Alexandru Csete, OZ9AEC.

  Authors: Alexandru Csete <oz9aec@gmail.com>

  Comments, questions and bugreports should be submitted via
  http://sourceforge.net/projects/gpredict/
  More details can be found at the project home page:

  http://gpredict.oz9aec.net/

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, visit http://www.fsf.org/
*/

#include <glib.h>
#include "sgpsdp/sgp4sdp4.h"
#include "gtk-sat-data.h"
#include "orbit-tools.h"
#include "time-tools.h"


/** \brief Initialise satellite data.
 *  \param sat The satellite to initialise.
 *  \param qth Optional QTH info, use (0,0) if NULL.
 *
 * This function calculates the satellite data at t = 0, ie. epoch time
 * The function is called automatically by gtk_sat_data_read_sat.
 */
void
gtk_sat_data_init_sat (sat_t *sat, qth_t *qth)
{
    geodetic_t obs_geodetic;
    obs_set_t obs_set;
    geodetic_t sat_geodetic;
    double jul_utc, age;

    g_return_if_fail (sat != NULL);

    jul_utc = Julian_Date_of_Epoch (sat->tle.epoch); // => tsince = 0.0
    sat->jul_epoch = jul_utc;

    /* initialise observer location */
    if (qth != NULL) {
        obs_geodetic.lon = qth->lon * de2ra;
        obs_geodetic.lat = qth->lat * de2ra;
        obs_geodetic.alt = qth->alt / 1000.0;
        obs_geodetic.theta = 0;
    }
    else {
        obs_geodetic.lon = 0.0;
        obs_geodetic.lat = 0.0;
        obs_geodetic.alt = 0.0;
        obs_geodetic.theta = 0;
    }

    /* execute computations */
    if (sat->flags & DEEP_SPACE_EPHEM_FLAG)
        SDP4 (sat, 0.0);
    else
        SGP4 (sat, 0.0);

    /* scale position and velocity to km and km/sec */
    Convert_Sat_State (&sat->pos, &sat->vel);

    /* get the velocity of the satellite */
    Magnitude (&sat->vel);
    sat->velo = sat->vel.w;
    Calculate_Obs (jul_utc, &sat->pos, &sat->vel, &obs_geodetic, &obs_set);
    Calculate_LatLonAlt (jul_utc, &sat->pos, &sat_geodetic);

    while (sat_geodetic.lon < -pi)
        sat_geodetic.lon += twopi;

    while (sat_geodetic.lon > (pi))
        sat_geodetic.lon -= twopi;

    sat->az = Degrees (obs_set.az);
    sat->el = Degrees (obs_set.el);
    sat->range = obs_set.range;
    sat->range_rate = obs_set.range_rate;
    sat->ssplat = Degrees (sat_geodetic.lat);
    sat->ssplon = Degrees (sat_geodetic.lon);
    sat->alt = sat_geodetic.alt;
    sat->ma = Degrees (sat->phase);
    sat->ma *= 256.0/360.0;
    sat->footprint = 2.0 * xkmper * acos (xkmper/sat->pos.w);
    age = 0.0;
    sat->orbit = (long) floor((sat->tle.xno * xmnpda/twopi +
                               age * sat->tle.bstar * ae) * age +
                              sat->tle.xmo/twopi) + sat->tle.revnum - 1;

    /* orbit type */
    sat->otype = get_orbit_type (sat);
}

/** \brief Copy satellite data.
  * \param source Pointer to the source satellite to copy data from.
  * \param dest Pointer to the destination satellite to copy data into.
  * \param qth Pointer to the observer data (needed to initialize sat)
  *
  * This function copies the satellite data from a source sat_t structure into
  * the destination. The function copies the tle_t data and calls gtk_sat_data_inti_sat()
  * function for initializing the other fields.
  *
  */
void gtk_sat_data_copy_sat (const sat_t *source, sat_t *dest, qth_t *qth)
{
    guint i;

    g_return_if_fail ((source != NULL) && (dest != NULL));

    dest->tle.epoch = source->tle.epoch;
    dest->tle.epoch_year = source->tle.epoch_year;
    dest->tle.epoch_day = source->tle.epoch_day;
    dest->tle.epoch_fod = source->tle.epoch_fod;
    dest->tle.xndt2o = source->tle.xndt2o;
    dest->tle.xndd6o = source->tle.xndd6o;
    dest->tle.bstar = source->tle.bstar;
    dest->tle.xincl = source->tle.xincl;
    dest->tle.xnodeo = source->tle.xnodeo;
    dest->tle.eo = source->tle.eo;
    dest->tle.omegao = source->tle.omegao;
    dest->tle.xmo = source->tle.xmo;
    dest->tle.xno = source->tle.xno;
    dest->tle.catnr = source->tle.catnr;
    dest->tle.elset = source->tle.elset;
    dest->tle.revnum = source->tle.revnum;

    dest->name = g_strdup (source->name);
    dest->nickname = g_strdup (source->nickname);
    for (i = 0; i < 9; i++)
        dest->tle.idesg[i] = source->tle.idesg[i];

    dest->tle.status = source->tle.status;
    dest->tle.xincl1 = source->tle.xincl1;
    dest->tle.omegao1 = source->tle.omegao1;

    dest->otype = source->otype;

    /* very important */
    dest->flags = 0;
    select_ephemeris (dest);

    /* initialise variable fields */
    dest->jul_utc = 0.0;
    dest->tsince = 0.0;
    dest->az = 0.0;
    dest->el = 0.0;
    dest->range = 0.0;
    dest->range_rate = 0.0;
    dest->ra = 0.0;
    dest->dec = 0.0;
    dest->ssplat = 0.0;
    dest->ssplon = 0.0;
    dest->alt = 0.0;
    dest->velo = 0.0;
    dest->ma = 0.0;
    dest->footprint = 0.0;
    dest->phase = 0.0;
    dest->aos = 0.0;
    dest->los = 0.0;

    gtk_sat_data_init_sat (dest, qth);
}


/** \brief Free satellite data
 *  \param sat Pointer to the satellite data to free
 *
 * This function frees the memory that has been dyunamically allocated
 * when creating a new satellite object.
 */
void gtk_sat_data_free_sat(sat_t *sat)
{

     if (sat){
          if (sat->name){
               g_free(sat->name);
               sat->name=NULL;
          }
          if (sat->nickname){
               g_free(sat->nickname);
               sat->nickname=NULL;
          }
          if (sat->website){
               g_free(sat->website);
               sat->website=NULL;
          }

          g_free(sat);
     }
}
