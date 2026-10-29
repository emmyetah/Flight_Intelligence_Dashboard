#pragma once
#include <string>
#include <QtGlobal> // for qint

struct FlightData
{
    std::string icao24; //aircraft icao (permanent, tied to aircraft)
    std::string callsign; //(temporary, tied to flight/mission)
    std::string departureAirport;
    std::string arrivalAirport;
    qint64 firstSeen;
    qint64 lastSeen;
};