#pragma once
#include <vector>
#include "AirlineStats.h"
#include "RouteStats.h"

//a basic struct with airport statistics
struct AirportStats{
    int totalFlights = 0;
    int arrivals = 0;
    int departures = 0;
    int totalDelays = 0;
    int totalCancellations = 0;
    std::vector<AirlineStats> topAirlines;
    std::vector<RouteStats> topRoutes;
};
