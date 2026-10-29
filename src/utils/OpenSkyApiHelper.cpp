#include "OpenSkyApiHelper.h"

#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtGlobal>
#include <QString>


//Learning notes

//QT documentation says that you only need one QAccessManager for a whole project

OpenSkyApiHelper::OpenSkyApiHelper(QObject *parent) : QObject(parent){
    manager = new QNetworkAccessManager(this);
    clientId = qEnvironmentVariable("OPENSKY_CLIENT_ID");
    clientSecret = qEnvironmentVariable("OPENSKY_CLIENT_SECRET");
    
    connect(
        manager, //whos the sender
        &QNetworkAccessManager::finished, //how will you know its done (signal)
        this, //whose object fucniton should be called (receiever) 
        &OpenSkyApiHelper::replyFinished //what function needs to run when it's done
    ); 
    connect(this, &OpenSkyApiHelper::authenticationReady,
        this, [this]() {
            pendingFlightReplies = 2;
            requestArrivalsAirport(pendingAirportIcao, pendingBegin, pendingEnd);
            requestDeparturesAirport(pendingAirportIcao, pendingBegin, pendingEnd);
        }
    );
 }

 //
void OpenSkyApiHelper::fetchAirportFlights(const std::string& airportIcao, qint64 begin, qint64 end){
    
    if (busy) {
        emit networkErrorOccurred("A flight fetch is already in progress");
        return;
    }

    busy = true;
    
    pendingAirportIcao = airportIcao;
    pendingBegin = begin;
    pendingEnd = end;

    requestAccessToken();

    
}

void OpenSkyApiHelper::requestArrivalsAirport(const std::string& airportIcao, qint64 begin, qint64 end) {
    
    //build URL
    QUrl url("https://opensky-network.org/api/flights/arrival");
    
    //add airport begin + end params
    QUrlQuery query; 
    query.addQueryItem("airport", QString::fromStdString(airportIcao));
    query.addQueryItem("begin", QString::number(begin)); //need to add a helper funciton that returns correct start and end time, will think on how I want to implement this later
    query.addQueryItem("end", QString::number(end));
    url.setQuery(query);

    //create request
    if (accessToken.isEmpty()) {
        emit networkErrorOccurred("No access token available");
        return;
    }
    QNetworkRequest request(url);
    
    //add access token
    request.setRawHeader("Authorization", QByteArray("Bearer ") + accessToken.toUtf8());

    //send GET QNetworkRequest & tag reply
    QNetworkReply *reply = manager->get(request);
    reply->setProperty("requestType", static_cast<int>(RequestType::AirportArrivals));
    
    //store ICAO on pely
    reply->setProperty("icao", QString::fromStdString(airportIcao));

}
void OpenSkyApiHelper::requestDeparturesAirport(const std::string& airportIcao, qint64 begin, qint64 end){
    //same structure as above request funtion
    QUrl url ("https://opensky-network.org/api/flights/departure");

    QUrlQuery query;
    query.addQueryItem("airport", QString::fromStdString(airportIcao));
    query.addQueryItem("begin", QString::number(begin));
    query.addQueryItem("end", QString::number(end));
    url.setQuery(query);

    if (accessToken.isEmpty()) {
        emit networkErrorOccurred("No access token available");
        return;
    }
    QNetworkRequest request(url);
    request.setRawHeader("Authorization", QByteArray("Bearer ") + accessToken.toUtf8());
    QNetworkReply *reply = manager->get(request);
    reply->setProperty("requestType", static_cast<int>(RequestType::AirportDepartures));
    reply->setProperty("icao", QString::fromStdString(airportIcao));
}

std::vector<FlightData> OpenSkyApiHelper::parseFlightData(const QJsonArray &jsonFlights) {
    //create an empty vector to store the flightData objects
    std::vector<FlightData> flights;
    //turn flight JSON arrays into flightData objects,
    for (const QJsonValue &value : jsonFlights) {
        QJsonObject jsonFlight = value.toObject();

        //create a singular object and link the data to the respective fields
        FlightData flight;

        flight.icao24 = jsonFlight["icao24"].toString().toStdString();
        flight.callsign = jsonFlight["callsign"].toString().toStdString();
        flight.departureAirport = jsonFlight["estDepartureAirport"].toString().toStdString();
        flight.arrivalAirport = jsonFlight["estArrivalAirport"].toString().toStdString();
        flight.firstSeen = static_cast<qint64>(jsonFlight["firstSeen"].toInteger());
        flight.lastSeen = static_cast<qint64>(jsonFlight["lastSeen"].toInteger());

        flights.push_back(flight);
    }   

    return flights; 
    
}


//private
void OpenSkyApiHelper::requestAccessToken(){
    //set the target authentication URL
    QUrl url("https://auth.opensky-network.org/auth/realms/opensky-network/protocol/openid-connect/token");
    //then make the request usin g QNetwork Library
    QNetworkRequest request(url);
    //add arguments/headers to the requesr
    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded"
    );

    //create a data object
    QUrlQuery data;
    //add query items / arguments to the data object (required by opensky)
    data.addQueryItem("grant_type", "client_credentials");
    data.addQueryItem("client_id", clientId);
    data.addQueryItem("client_secret", clientSecret);
    //post the request via the manager pointer created to the url.
    QNetworkReply *reply = manager->post(request,data.toString(QUrl::FullyEncoded).toUtf8());
    reply->setProperty("requestType",static_cast<int>(RequestType::Token)); //set property can store integers so temporarily converting
}
void OpenSkyApiHelper::replyFinished(QNetworkReply *reply){
    reply->deleteLater(); // reccomended by Qt, schedules every reply for cleanup
    RequestType requestType = static_cast<RequestType>(reply->property("requestType").toInt());
    if (requestType == RequestType::AirportArrivals || requestType == RequestType::AirportDepartures) {
        --pendingFlightReplies;
        if (pendingFlightReplies == 0) {
            busy = false;
        }
    }
    if (reply->error() != QNetworkReply::NoError) {
        if (requestType == RequestType::Token) {
                busy = false;
            }
        //handle error here, if it fails we don't want to parse it.
        emit networkErrorOccurred(reply->errorString());
        return;
    }
    //step 2 is to read the response into a Qbyte array
    QByteArray data = reply->readAll();    

    switch (requestType) {
    case RequestType::Token:
    { 
        //next is parsing the response to extract the token.
        //after researching i found that Qt already has a JSON parser, safer to use thin in substr() as JSON formatting can change.
        QJsonDocument document = QJsonDocument::fromJson(data);
        QJsonObject object = document.object();

        //quick check before storing a token, in case it's empty
        if (!object.contains("access_token") || !object["access_token"].isString()) {
            busy = false;
            emit networkErrorOccurred("Access token missing from response");
            return;
        }
        
        //finally, store access token- 
        accessToken = object["access_token"].toString();

        if (accessToken.isEmpty()) {
            busy = false;
            emit networkErrorOccurred("No access token available");
            return;
        }

        emit authenticationReady();
        break;
    }

    case RequestType::AirportArrivals:
    {   
        QJsonParseError error;

        //parses the response as JSON
        QJsonDocument document = QJsonDocument::fromJson(data, &error);

        //checks if JSON parser hit a syntax error or has an incorrect format
        if (error.error != QJsonParseError::NoError || !document.isArray()) {
            emit networkErrorOccurred("Invalid flight response");
            return;
        }

        //extracts array of flight objects
        QJsonArray flights = document.array();
       
        //parses flight objects are in the array
        auto arrivals = parseFlightData(flights);
        //gets the icao code to store with the number of flights as a refernece
        std::string icao = reply->property("icao").toString().toStdString();
        //tells us the reply is here and the data is ready to be used
        emit arrivalsReady(icao, arrivals);

        break;
    }

    case RequestType::AirportDepartures:
    {
        //same structure as above case
        QJsonParseError error;
        QJsonDocument document = QJsonDocument::fromJson(data, &error);
        if (error.error != QJsonParseError::NoError || !document.isArray()) {
            emit networkErrorOccurred("Invalid flight response");
            return;
        }
        QJsonArray flights = document.array();

        auto departures = parseFlightData(flights);

        std::string icao = reply->property("icao").toString().toStdString();
        emit departuresReady(icao,departures);

        break;
    }

    default:
    { 
        emit networkErrorOccurred("Unknown request type");
        break;
    }
    return;
    
    }
}



