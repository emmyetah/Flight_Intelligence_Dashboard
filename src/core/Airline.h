#pragma once
#include <string>
//I believe this is the only airline-specific infomration I will need right now

Class Airline {
    private:
        std::string IcaoAirlineCode;
        std::string name;
    public:
        Airline (
            const std::string& Icao,
            const std::string& name
        )

    std::string getAirlineIcao() const;
    std::string getAirlineName() const;
}