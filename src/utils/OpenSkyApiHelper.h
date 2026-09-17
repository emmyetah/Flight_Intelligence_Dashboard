#pragma once

#include "../core/FlightData.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <string>
#include <vector>

//main objective of this header file is to make a simple way for my service to intercat with the api
//I'll be using QtNetwork as my client ot connect to the REST OpenSky API.

//declaring class as a Q object, giving it access to Q object classes & fucntions
class OpenSkyApiHelper : public QObject {
    Q_OBJECT 

public: 
    //constructor declaration
    OpenSkyApiHelper(QObject *parent = nullptr);

    //request fucntions in dcelared publically as they'll be used by my services
    //originally the data types were correct and not all void (int, std::vector etc) but I foudn that becuase QTNetwokr is asynchronous, I have to pass void then the signals will return the correct data type later.
    void requestArrivalsAirport(const std::string& AirportIcao);
    void requestDeparturesAirport(const std::string& AirportIcao);
    void requestArrivalsAirline(const std::string& AirlineIcao);
    void requestDeparturesAirline(const std::string& AirlineIcao);
    void requestAirportFlights(const std::string& AirportIcao);
signals:
    //signals notify connected classes when requested data is ready and pass data to receiver.
    void arrivalsReady(const std::string &icao, int arrivals);
    void departuresReady(const std::string &icao,int departures);
    void flightsReady(const std::vector<FlightData> &flights)
private: 
    //declaring the manager pointer. Rule of thumb, use pointers not refernces or copies 
    QNetworkAccessManager *manager;
    void requestAccessToken();
    //processes the response e.g., parse, check for errors etc.
    void replyFinished(QNetworkReply *reply);
    void handleNetworkError(QNetworkReply  *reply);
    void parseFlightData(const QByteArray &data); 
} ;