Prerequsites:

boost-program-options  (command line parser)

    sudo apt install libboost-program-options-dev  

udunits2 physical units package (converts distance strings to km)

    sudo apt install libudunits2-dev
    
    
Files:

> Requested C methods for coordinate transform

>> `GisToRadar.c` 

> C++ CLI	
>> `main.cpp`

To build:

> Clone repo.
>
> Make default target, by just typing “make.”

>> (or with explicit target "make CoordTrans")

To use:

```shell
./CoordTrans 
usage: CoordTrans <command> <args>, command is either gis-to-radar, or radar-to-gis:
  -? [ --help ]                This help message
  -r [ --range ] arg           Radar indicated distance.
                               "arg" is distance with optional unit suffix
  -b [ --bearing ] arg         Radar indicated direction.
                               "arg" is angle in decimal degrees
  -S [ --start-latitude ] arg  Radar latitude.
                               "arg" is angle in decimal degrees
  -s [ --start-longitude ] arg Radar longitude.
                               "arg" is angle in decimal degrees
  -E [ --end-latitude ] arg    Target latitude for gis-to-radar.
                               "arg" is angle in decimal degrees
  -e [ --end-longitude ] arg   target longitude for gis-to-radar.
                               "arg" is angle in decimal degrees
                               
./CoordTrans gis-to-radar -S 37 -s "-75" -E 18 -e "-66"
Range: 2288.66 km Bearing:  154.9632 deg.

./CoordTrans radar-to-gis -S 37 -s "-75" -r "2288.66 km" -b 154.9632
Latitude:   18.0000 deg. Longitude:  -66.0000 deg.

./CoordTrans radar-to-gis -S 37 -s "-75" -r 1422.107mi -b 154.9632
Latitude:   18.0000 deg. Longitude:  -66.0000 deg.

./CoordTrans radar-to-gis -S 37 -s "-75" -r 228866000cm -b 154.9632
Latitude:   18.0000 deg. Longitude:  -66.0000 deg.

./CoordTrans radar-to-gis -S 37 -s "-75" -r 1235.777nautical_mile -b 154.9632
Latitude:   18.0000 deg. Longitude:  -66.0000 deg.

./CoordTrans radar-to-gis -S 37 -s "-75" -r 600nautical_mile -b 180
Latitude:   27.0067 deg. Longitude:  -75.0000 deg.
```

Assumptions 

>Question clearly indicates radar does not take a straight line-of-sight route given that suggested end points are far beyond each others' horizons.  I have implemented the standard 19th century spherical trigonometric haversine formulas from the age of sail.  These would be exactly correct if radar traveled entirely at sea level across a planet of Earth's size that was perfectly spherical.  None of these assumptions are 100% applicable to radar measurements on real Earth, but I do not know how to alter them to make them more correct.  Radar I guess would be channeled through a shell with floor at surface and ceiling in Ionosphere, this could be modeled by altering the global earth radius constant in the GisToRadar.c file.  Given that radar travels in some shell that isn't the surface of Earth, conventional corrections for ellipticity of true Earth shape may not be relevant.

Potential Improvements

>Unidata udUnits2 is a mature units library for C unit handling, and it is easy to pull from package manager, but it is not the most natural or robust when it comes to interpreting common abbreviations.  E.g. a reasonable abbreviation for "nautical mile" might be "nmi" but udUnits2 parsing rules require this to be parsed as nano-miles which is consistent but not useful interpretation.  I think there are other libraries more suitable for our problem in the wild, but time did not permit me to root them out.

>If nautical miles are to be supported, A "DDD MM SS.ss" input parser for lat/lon would be a good complement, but again time didn't permit exploring such.

>Finally, in hindsight this utility could be made into a nice Unix style command line tool if I had devoted more effort into a mode that takes bulk input from stdin and writes a streamable format out so it would work as a part of a pipe mill.  Didn't think of that until just now. 