#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "prototypes.h"

int main(int argc, char **argv)
{
    return 0;
}

const double g_EarthRadius = 6371000;

const int SUCCESS = 0;

double pi = 3.1416926;

static inline double radian( double theta ) { return pi * theta / 180; }

static inline double degree( double theta ) { return 180 * theta / pi; }

double haversine( double theta) {  return (1 - cos(theta)) / 2; }

int GIStoRadar(double *range,
               double *bearing,
               double glonInit,
               double glatInit,
               double glonFinal,
               double glatFinal )
{
    double delta_phi = radian( glatFinal - glatInit ), delta_lambda = radian( glonFinal - glonInit ),
            phi_init = radian( glatInit ), phi_final = radian( glatFinal );

    double haversine_theta = haversine(delta_phi) + cos(phi_init)*cos(phi_final)*haversine(delta_lambda);

    *range = 2 * g_EarthRadius * asin( sqrt( haversine_theta ) );

    return SUCCESS;
}

int RadartoGis(double range,
               double bearing,
               double  glonInit,
               double  glatInit,
               double *glonFinal,
               double *glatFinal)
{
    return SUCCESS;
}
