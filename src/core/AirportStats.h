#pragma once
#include <vector>
#include "AirlineStats.h"
#include "RouteStats.h"

//a basic struct with airport statistics
struct AirportStats{
    int totalFlights;
    int arrivals;
    int departures;
    int totalDelays;
    int totalCancellations;
    std::vector<AirlineStats> topAirlines;
    std::vector<RouteStats> topRoutes;
};
