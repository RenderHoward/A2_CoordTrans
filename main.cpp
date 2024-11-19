#include <iostream>
#include <boost/program_options.hpp>
#include <boost/format.hpp>
#include <udunits2.h>
namespace poptions = boost::program_options;
#include <iterator>
using namespace std;
#include "prototypes.h"

class DistanceConverter
{
    ut_system* m_UnitSystem;

    ut_unit* m_km;

    public:
    DistanceConverter()
    {
        ut_set_error_message_handler(ut_ignore);
        m_UnitSystem = ut_read_xml(NULL);
        ut_set_error_message_handler(ut_write_to_stderr);

        m_km = ut_parse(m_UnitSystem, "km", UT_ASCII);
    }

    bool CanConvert(std::string string)
    {
        bool result = false;

        ut_unit* in = ut_parse(m_UnitSystem, string.c_str(), UT_ASCII);

        if(in)
            result = ut_are_convertible(in, m_km);

        auto nm = ut_get_name(in, UT_ASCII);

        ut_free(in);

        return result;
    }

    double ConvertToKilometers(std::string string)
    {
        ut_unit* in = ut_parse(m_UnitSystem, string.c_str(), UT_ASCII);

        cv_converter* converter = ut_get_converter(in, m_km);

        auto answer = cv_convert_double(converter, 1.0);

        cv_free(converter);
        ut_free(in);

        return answer;
    }
};

static auto g_converter = DistanceConverter();

int main(int argc, char *argv[])
{
    double range, bearing, lat1, lon1, lat2, lon2;

    std::string command;

    try {
            std::string called_path(argv[0]);
            std::string base_filename = called_path.substr(called_path.find_last_of("/\\") + 1);
            auto example_cmd_line = boost::format("usage: %1% <command> <args>, command is either gis-to-radar, or radar-to-gis") % base_filename;

            poptions::options_description desc( example_cmd_line.str() );

            int opt;

            desc.add_options()
                ("help,?", "This help message")
                ("range,r", poptions::value<string>(), "Radar indicated distance.\n\"arg\" is distance with optional unit suffix")
                ("bearing,b", poptions::value<double>(), "Radar indicated direction.\n\"arg\" is angle in decimal degrees")
                ("start-latitude,S",  poptions::value<double>(), "Radar latitude.\n\"arg\" is angle in decimal degrees")
                ("start-longitude,s", poptions::value<double>(), "Radar longitude.\n\"arg\" is angle in decimal degrees")
                ("end-latitude,E",  poptions::value<double>(), "Target latitude for gis-to-radar.\n\"arg\" is angle in decimal degrees")
                ("end-longitude,e", poptions::value<double>(), "target longitude for gis-to-radar.\n\"arg\" is angle in decimal degrees");

            // To prevent positional arguments from showing in usage blurb
            poptions::options_description hidden_desc;
            hidden_desc.add_options()
                ("command", poptions::value<string>(), "Either radar-to-gis, or gis-to-radar");


            poptions::positional_options_description command_desc;
            command_desc.add("command", 1);

            poptions::options_description all_options;
            all_options.add(desc).add(hidden_desc);


            poptions::variables_map vm;
            poptions::store(poptions::command_line_parser(argc, argv).
                            options(all_options).
                            positional(command_desc).
                            run(), vm);

            poptions::notify(vm);

            if( vm.size() == 0 )
            {
                cout << desc << "\n";
                return 0;
            }

            if( vm.count("help") )
            {
                cout << desc << "\n";
                return 0;
            }

        if (vm.count("command"))
            command = vm["command"].as<string>();
        else
            cout << "command was not set.\n";

        if (vm.count("range"))
        {
            auto arg = vm["range"].as<string>();

            // check if no units
            char *end;

            double dbl = strtod(arg.c_str(), &end);

            if(*end)
            {
                if( g_converter.CanConvert( arg ) )
                    range = g_converter.ConvertToKilometers( arg );
            }
            else
                    range = dbl;
        }

        if (vm.count("bearing"))
            bearing = vm["bearing"].as<double>();

        if (vm.count("start-latitude"))
            lat1 = vm["start-latitude"].as<double>();

        if (vm.count("start-longitude"))
            lon1 = vm["start-longitude"].as<double>();

        if (vm.count("end-latitude"))
            lat2 = vm["end-latitude"].as<double>();

        if (vm.count("end-longitude"))
            lon2 = vm["end-longitude"].as<double>();

        if( command.compare("gis-to-radar") == 0 )
        {
            if
            (
                vm.count("start-latitude")  != 1  ||
                vm.count("start-longitude") != 1  ||
                vm.count("end-latitude")    != 1  ||
                vm.count("end-longitude")   != 1
            )
            {
                cout << "Incorrect parameters for " << command;
                return -1;
            }

            GIStoRadar( &range, &bearing, lon1, lat1, lon2, lat2 );

            auto output = (boost::format("Range: %1$7.2f km Bearing: %2$9.4f deg.\n") % range % bearing).str();

            cout << output;
        }

        if( command.compare("radar-to-gis") == 0 )
        {
            if
            (
                vm.count("start-latitude")  != 1    ||
                vm.count("start-longitude") != 1    ||
                vm.count("bearing")         != 1    ||
                vm.count("range")           != 1
            )
            {
                cout << "Incorrect parameters for " << command;
                return -1;
            }

            RadartoGIS( range, bearing, lon1, lat1, &lon2, &lat2);

            auto output = (boost::format("Latitude: %1$9.4f deg. Longitude: %2$9.4f deg.\n") % lat2 % lon2).str();

            cout << output;
        }

    }
    catch(exception& e)
    {
        cerr << "error: " << e.what() << "\n";
        return 1;
    }
    catch(...)
    {
        cerr << "Exception of unknown type!\n";
    }

    return 0;
}

