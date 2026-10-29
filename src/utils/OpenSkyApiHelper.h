#pragma once

#include "../core/FlightData.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QString>
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

    //originally the data types were correct and not all void (int, std::vector etc) but I foudn that becuase QTNetwokr is asynchronous, I pass void then the signals will return the correct data type later.
    //MOved request functions to private as the fetch function returns both arrivals and departure data
    void fetchAirportFlights(const std::string& airportIcao, qint64 begin, qint64 end);
signals:
    //signals notify connected classes when requested data is ready and pass data to receiver.
    void authenticationReady();
    void arrivalsReady(const std::string &icao, const std::vector<FlightData> &arrivals);
    void departuresReady(const std::string &icao, const std::vector<FlightData> &departures);

    void networkErrorOccurred(const QString &message); //adding prominent erros here, I can emit then in later functions to pass onto other parts of my system wihtout crashes.
private: 
    //declaring the manager pointer
    QNetworkAccessManager *manager;
    //for accessing the opensky API
    QString clientId;
    QString clientSecret;
    QString accessToken;

    //for storing the request airport and times
    std::string pendingAirportIcao;
    qint64 pendingBegin = 0;
    qint64 pendingEnd = 0;

    //for state checking
    bool busy = false;
    int pendingFlightReplies = 0;
    
    void requestAccessToken();
    void requestArrivalsAirport(const std::string& AirportIcao, qint64 begin, qint64 end);
    void requestDeparturesAirport(const std::string& AirportIcao, qint64 begin, qint64 end);
    
    //processes the response e.g., parse, check for errors etc.
    void replyFinished(QNetworkReply *reply);
    std::vector<FlightData> parseFlightData(const QJsonArray &jsonFlights);


    enum class RequestType {
        Unknown,
        Token,
        AirportArrivals,
        AirportDepartures
    };
} ;