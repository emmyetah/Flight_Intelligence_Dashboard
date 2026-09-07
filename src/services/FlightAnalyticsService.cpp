#include "FlightAnalyticsService.h"
#include<string>

 FlightAnalyticsService::FlightAnalyticsService{
    //vars
    
    //fns
    int FlightAnalyticsService::calcTotalFlightsAirline(string airport, string airlineIcao) {
        //get complete number of airline flights using api
        //takes arguments, puts them into api connector function & returns number of flights for that airline
    }
    int FlightAnalyticsService::calcTotalFlightsAirport(string airport) {
        //use opensky to get data for total arrivals & total departures via airport, then add together
        //obviously will be completed in another class then imported
        //opensky doesn't allow us to calculate via airline. maybe look into aviationstack for this
    }

    int FlightAnalyticsService::calcTotalDelaysAirline() {
        //may need to use aviationstack api to calculate these. can leave for next verison
    }
    int cFlightAnalyticsService::alcTotalDelaysAirport() {
        //may need to use aviationstack api to calculate these. can leave for next verison
    }

    int FlightAnalyticsService::calcTotalCancellationsAirline(string month, string airport) {
        //need to make a csv reader class
        //CAA provides csv data of cancelled flights from each aiport for a specific month
        //or I could use the aviationstack API to get cancellation data.  Free plan 100RQ per month
    }
    int FlightAnalyticsService::calcTotalCancellationsAirport() {

    }


    vector FlightAnalyticsService::returnTopFiveAirlines(string airport) {
        //returns a vector with airlines that have the most flights
        //e.g., JET2: 84, british airways: 73 etc
    }
    vector FlightAnalyticsService::returnTopFiveRoutes(string airport) {
        //returns the top5 routes for each airport
        //e.g., MAN->BCN 12, LHR->MAD 7 etc
    }

    vector FlightAnalyticsService::returnAirlineStats(string airline) {
        //calculates all of the statistics above for a specific airlien where applicable then return vector with results
        calcTotalFlightsAirline();
        calcTotalDelaysAirline();
        calcTotalFlightsAirline();

    }

    void FlightAnalyticsService::returnAirportStats(string airport) {
        //same as above but specific to the airport not airlines
        int a = = calcTotalCancellationsAirport();
        int b = calcTotalDelaysAirport();
        int c = calcTotalFlightsAirport();
        vector d = returnTopFiveAirlines();
        vector e = returnTopFiveRoutes();
        //then returns structures data for GUI controllers
        
    }

}
