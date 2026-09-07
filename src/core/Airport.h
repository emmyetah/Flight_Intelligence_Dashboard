#pragma once
#include <string>

//this class is for storing information about the airports
Class Airport {
    
    private:
        std::string icaoCode; // e.g., EGLL 
        std::string iataCode; //e.g., MAN
        std::string name;
        std::string city;
    
    public:
        //constructor
        Airport (
            const std::string& icao,
            const std::string& iata,
            const std::string& name,
            const std::string& city
        );
        
        //getters - added amersand symbols to prevent copies
        std::string& getIcaoCode() const;
        std::string& getIatacode() const;
        std::string& getName() const;
        std::string& getCity() const;

};