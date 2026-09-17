#pragma once
#include <string>
#include <QtGlobal> // for qint

struct FlightData
{
    std::string callsign;
    std::string departureAirport;
    std::string arrivalAirport;
    qint64 firstSeen;
    qint64 lastSeen;
};