#include <stdlib.h>
#include <stdio.h>
#include "../gpredict.h"

char  tle_str[3][80];
sat_t sat;
qth_t qth;

/* timestamp: 1367358140; */
gdouble pass_time   = 2456413.4043981;

/* expected results */
gdouble expected_lat;

int main () {

    printf ("\nTesting a single ISS prediction\n");

    /* Initialize TLE */
    strcpy(tle_str[0], "ISS (ZARYA)");
    strcpy(tle_str[1], "1 25544U 98067A   13119.87718750  .00010964  00000-0  18836-3 0  7119");
    strcpy(tle_str[2], "2 25544  51.6469 347.9617 0008088 218.1847 316.4186 15.51658541827204");
    if (Get_Next_Tle_Set (tle_str, &sat.tle) == 1) {
        printf ("\nTLE Data OK\n\n");
    } else {
        printf ("\nTLE NOT OK, exiting\n");
        return 1;
    }

    /* Initialize QTH */
    qth.lat = 39.164141;
    qth.lon = -122.695312;
    qth.alt = 0;

    /* Initialize Satellite */
    select_ephemeris (&sat);
    gtk_sat_data_init_sat(&sat, &qth);

    /* Calculate Position for specified date */
    predict_calc(&sat, &qth, pass_time);

    /* Display results */
    printf("Lat:       %10.12f   (expected:  18.389606541458)\n", sat.ssplat);
    printf("Lon:       %10.12f  (expected: -36.879846099166)\n", sat.ssplon);
    printf("Alt:        %4.10f   (expected: 409.7429029679)\n", sat.alt);
    printf("Footprint:  %4.10f  (expected: 4454.9074167831)\n", sat.footprint);
    printf("Velocity:  %10.10f      (expected: 7.66919316690)\n", sat.velo);

    return 0;
}
