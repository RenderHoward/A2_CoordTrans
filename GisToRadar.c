#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "prototypes.h"

const double g_EarthRadius = 6371;

const int SUCCESS = 0;

const int ERROR = -1;

const double pi = 3.1415926;

static inline double radian( double theta ) { return pi * theta / 180; }

static inline double degree( double theta ) { return 180 * theta / pi; }

static inline double haversine( double theta) {  return (1 - cos(theta)) / 2; }

int GIStoRadar(double *range,
               double *bearing,
               double glonInit,
               double glatInit,
               double glonFinal,
               double glatFinal )
{
    if( bearing == NULL || range == NULL )
        return ERROR;

    double delta_phi = radian( glatFinal - glatInit ), delta_lambda = radian( glonFinal - glonInit ),
            phi_init = radian( glatInit ), phi_final = radian( glatFinal );

    double haversine_theta = haversine(delta_phi) + cos(phi_init)*cos(phi_final)*haversine(delta_lambda);

    *range = 2 * g_EarthRadius * asin( sqrt( haversine_theta ) );

    *bearing =
        degree
        (
            atan2
            (
                sin(delta_lambda)*cos(phi_final),
                cos(phi_init)*sin(phi_final) - sin(phi_init)*cos(phi_final)*cos(delta_lambda)
            )
        );

    return SUCCESS;
}

int RadartoGIS(double range,
               double bearing,
               double  glonInit,
               double  glatInit,
               double *glonFinal,
               double *glatFinal)
{
    if( glonFinal == NULL || glatFinal == NULL )
        return ERROR;

    double delta = range / g_EarthRadius;   // Central angle along great circle

    double phi_init = radian(glatInit), bearad = radian(bearing),
           phi_final = asin( sin(phi_init) * cos(delta) + cos(phi_init) * sin(delta) * cos(bearad));

    *glatFinal = degree( phi_final );

    double lambda_init = radian(glonInit);

    *glonFinal = degree
                 (
                    lambda_init +
                    atan2
                    (
                        sin(radian(bearing)) * sin(delta) * cos(phi_init),
                        cos(delta) - sin(phi_init)*sin(phi_final)
                    )
                );

    return SUCCESS;
}
