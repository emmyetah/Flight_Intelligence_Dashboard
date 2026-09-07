#pragma once
#include <string>
//I believe this is the only airline-specific infomration I will need right now

Class Airline {
    private:
        std::string icaoAirlineCode; 
        std::string name;
    public:
        Airline (
            const std::string& icao,
            const std::string& name
        );

    //added ampersand symbols to prevent copies & improve memory efficiency    
    std::string& getAirlineIcao() const;
    std::string& getAirlineName() const;
};