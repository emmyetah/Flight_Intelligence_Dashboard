#pragma once

struct AirlineStats {
    //added initialisers to prevent using garbage data from memory slot 
    int totalAirlineFlights = 0;
    int totalDelays = 0;
    int totalCancellations = 0;
}; 