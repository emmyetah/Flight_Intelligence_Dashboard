#pragma once
#include <string>

struct RouteStats {
    //two airport codes are needed for the route 
    std::string originAirportCode;
    std::string destinationAirportCode;

    //also initialised flightcount var to prevent garbage data
    int flightCount = 0;
};