#pragma once

Class FlightAnalyticsService {
    public:

    int calcTotalFlightsAirport();
    int calcTotalFlightsAirline();
    int calcTotalDelaysAirport();
    int calcTotalDelaysAirline();
    int calcTotalCancellationsAirport();
    int calcTotalCancellationsAirline();
    vector returnTopFiveAirlines();
    vector returnTopFiveRoutes();
    vector returnAirlineStats();
    vector returnAirportStats();
}