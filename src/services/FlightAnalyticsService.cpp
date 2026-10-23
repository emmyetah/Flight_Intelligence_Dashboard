#include "FlightAnalyticsService.h"
#include <string>
#include <vector>

 FlightAnalyticsService::FlightAnalyticsService() {

 }
    //vars
    
    //fns
    int FlightAnalyticsService::calcTotalFlightsAirline(const std::string& airport, const std::string& airlineIcao) {
        //get complete number of airline flights using api
        //takes arguments, puSts them into api connector function & returns number of flights for that airline
        return 0;
    }
    int FlightAnalyticsService::calcTotalFlightsAirport(const std::string& airport) {
        //use opensky to get data for total arrivals & total departures via airport, then add together
        //obviously will be completed in another class then imported
        //opensky doesn't allow us to calculate via airline. maybe look into aviationstack for this
        return 0;
    }

    int FlightAnalyticsService::calcTotalDelaysAirline(const std::string&  airlineIcao, const std::string&  airportIcaoc) {
        //may need to use aviationstack api to calculate these. can leave for next verison
        return 0;
    }   
    int FlightAnalyticsService::calcTotalDelaysAirport(const std::string&  airportIcao) {
        //may need to use aviationstack api to calculate these. can leave for next verison
        return 0;
    }

    int FlightAnalyticsService::calcTotalCancellationsAirline(const std::string&  airlineIcao, const std::string&  airportIcao) {
        //need to make a csv reader class
        //CAA provides csv data of cancelled flights from each aiport for a specific month
        //or I could use the aviationstack API to get cancellation data.  Free plan 100RQ per month
        return 0;
    }
    int FlightAnalyticsService::calcTotalCancellationsAirport(const std::string&  airportIcao) {
        return 0;
    }


    std::vector<AirlineStats> FlightAnalyticsService::returnTopFiveAirlines(const std::string& airport) {
        //returns a vector with airlines that have the most flights
        //e.g., JET2: 84, british airways: 73 etc
        //might need to use another utils fucntion to filter and sort these
        std::vector<AirlineStats> flights;
        return flights;
    }
    std::vector<RouteStats> FlightAnalyticsService::returnTopFiveRoutes(const std::string& airport) {
        //returns the top5 routes for each airport
        //e.g., MAN->BCN 12, LHR->MAD 7 etc
        std::vector<RouteStats> flights;
        return flights;
    }

    AirlineStats FlightAnalyticsService::returnAirlineStats(const std::string&  airlineIcao, const std::string& airportIcao) {
        //calculates all of the statistics above for a specific airlien where applicable then return vector with results
        //calcTotalFlightsAirline();
        //calcTotalDelaysAirline();
        //calcTotalFlightsAirline();
        AirlineStats flights;
        return flights;

    }

    AirportStats FlightAnalyticsService::returnAirportStats(const std::string& airport) {
        //same as above but specific to the airport not airlines
        //calcTotalCancellationsAirport();
        //calcTotalDelaysAirport();
        //calcTotalFlightsAirport();
        //calcTotalDepartures();
        //calcTotalArrivals();
       // returnTopFiveAirlines();
        //returnTopFiveRoutes();
        //then returns structures data for GUI controllers

        AirportStats flights;
        return flights;
        
    }

