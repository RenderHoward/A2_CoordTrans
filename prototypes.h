#ifndef PROTOTYPES_H
#define PROTOTYPES_H

#ifdef __cplusplus
extern "C" {
#endif

int GIStoRadar( double *range,
                double *bearing,
                double  glonInit,
                double  glatInit,
                double  glonFinal,
                double  glatFinal );

int RadartoGIS( double  range,
                double  bearing,
                double  glonInit,
                double  glatInit,
                double *glonFinal,
                double *glatFinal);

#ifdef __cplusplus
}
#endif

#endif // PROTOTYPES_H
