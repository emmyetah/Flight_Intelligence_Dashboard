#pragma once
#include<string>
#include<vector>

#include "../core/RouteStats.h"
#include "../core/AirportStats.h"
#include "../core/AirlineStats.h"


Class FlightAnalyticsService {
    private:
    //decided to make all the calculation functions private as they're internal implementation details
    //Also, all airline specific functions need a specified airpot - added

        int calcTotalFlightsAirport(const std::string& airportIcao);
        int calcTotalFlightsAirline(const std::string&  airlineIcao, const std::string& airportIcao);
        int calcTotalArrivals(const std::string&  airportIcao);
        int calcTotalDepartures(const std::string&  airportIcao);
        int calcTotalDelaysAirport(const std::string&  airportIcao);
        int calcTotalDelaysAirline(const std::string&  airlineIcao, const std::string&  airportIcao);
        int calcTotalCancellationsAirport(const std::string&  airportIcao);
        int calcTotalCancellationsAirline(const std::string&  airlineIcao, const std::string&  airportIcao);
        std::vector<AirlineStats> returnTopFiveAirlines(const std::string&  airportIcao);
        std::vector<RouteStats> returnTopFiveRoutes(const std::string&  airportIcao);

    public:
        
        AirlineStats returnAirlineStats(const std::string&  airlineIcao, const std::string& airportIcao);
        AirportStats returnAirportStats(const std::string&  airportIcao);
};
//all icao codes need to be passed through, not copies, so i made them constants to prevent modification and used the ambersand sign