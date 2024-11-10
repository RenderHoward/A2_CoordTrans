#ifndef PROTOTYPES_H
#define PROTOTYPES_H

int GIStoRadar( double *range,
                double *bearing,
                double glonInit,
                double glatInit,
                double glonFinal,
                double glatFinal );

int RadartoGIS( double range,
                double bearing,
                double  glonInit,
                double glatInit,
                double *glonFinal,
                double *glatFinal);


#endif // PROTOTYPES_H
