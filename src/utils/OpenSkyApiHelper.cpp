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
        &OpenSkyApiHelper::replyFinished); //what function needs to run when it's done
    }

    //QUrl creates a structured url form the string you put in it. The ->get is telling the manager fucniton to send the HTTP Request using GET
    //use this line in requests manager->get(QNetworkRequest(QUrl(""))); // this will return a QNetworkReply*


//public fns
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

void OpenSkyApiHelper::requestAirportFlights(const std::string& AirportIcao){

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
    data.addQueryItem("client_secrt", clientSecret);
    //post the request via the manager pointer created to the url.
    manager->post(request,data.toString(QUrl::FullyEncoded).toUtf8());
    QNetworkReply *reply = manager->get(request);
    reply->setProperty("requestType",static_cast<int>(RequestType::Token)); //set property can store integers so temporarily converting
}
void OpenSkyApiHelper::replyFinished(QNetworkReply *reply){
    if (reply->error() != QNetworkReply::NoError) {
        //handle error here, if it fails we don't want to parse it.
        emit networkErrorOccurred(reply->errorString());
        return;
    }
    //step 2 is to read the response into a Qbyte array
    QByteArray data = reply->readAll();    

    RequestType requestType = static_cast<RequestType> (
        reply->property("requestType").toInt()
    );

     switch (requestType) {

    case RequestType::Token:
    { 
        //next is parsing the response to extract the token.
        //after researching i found that Qt already has a JSON parser, safer to use thin in substr() as JSON formatting can change.
        QJsonDocument document = QJsonDocument::fromJson(data);
        QJsonObject object = document.object();

        //quick check before storing a token, in case it's empty
        if (!object.contains("access_token") || !object["access_token"].isString()) {
            emit networkErrorOccurred("Access token missing from response");
            return;
        }
    
        //finally, store access token- 
        accessToken = object["access_token"].toString();
        return; 
        break;
    }
    case RequestType::AirportFlights:
    {
        parseFlightData(data);
        break;
    }

    case RequestType::AirportArrivals:
    { 
        //parses the response as JSON
        QJsonDocument document = QJsonDocument::fromJson(data);
        //extracts array of flight objects
        QJsonArray flights = document.array();

        //counts how many flight objects are in the arrau
        int arrivals = flights.size();

        //gets the icao code to store with the number of flights as a refernece
        std::string icao = reply->property("icao").toString().toStdString();
        //tells us the reply is here and the data is ready to be used
        emit arrivalsReady(icao, arrivals);

    break;
    }

    case RequestType::AirportDepartures:
    {
        //same structure as above case
        QJsonDocument document = QJsonDocument::fromJson(data);
        QJsonArray flights = document.array();
        int departures = flights.size();

        std::string icao = reply->property("icao").toString().toStdString();
        emit arrivalsReady(icao,departures);

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


void OpenSkyApiHelper::parseFlightData(const QByteArray &data){

}


