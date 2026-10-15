#include "OpenSkyApiHelper.h"

#include <QUrl>
#include <QurlQuery>
#include <QJsonDocument>
#include <QJsonObject>


//Learning notes

//QT documentation says that you only need one QAccessManager for a whole project

OpenSkyApiHelper::OpenSkyApiHelper(QObject *parent) : QObject(parent){
    manager = new QNetworkAccessManager(this);
    clientId = qEnironmentVariable("OPENSKY_CLIENT_ID");
    clientSecret = qEnironmentVariable("OPENSKY_CLIENT_SECRET");
    
    connect(
        manager, //whos the sender
        &QNetworkAccessManager::finished, //how will you know its done (signal)
        this, //whose object fucniton should be called (receiever) 
        &OpenSkyApiHelper::replyFinished); //what function needs to run when it's done
    }

    //QUrl creates a structured url form the string you put in it. The ->get is telling the manager fucniton to send the HTTP Request using GET
    //use this line in requests manager->get(QNetworkRequest(QUrl(""))); // this will return a QNetworkReply*


//public fns
void OpenSkyApiHelper::requestArrivalsAirport(const std::string& AirportIcao) {

}
void OpenSkyApiHelper::requestDeparturesAirport(const std::string& AirportIcao){

}
void OpenSkyApiHelper::requestArrivalsAirline(const std::string& AirlineIcao){

}
void OpenSkyApiHelper::requestDeparturesAirline(const std::string& AirlineIcao){
    
}
void OpenSkyApiHelper::requestAirportFlights(const std::string& AirportIcao){

}

//private
void OpenSkyApiHelper::requestAccessToken(){
    //set the target authentication URL
    QUrl url = ("https://auth.opensky-network.org/auth/realms/opensky-network/protocol/openid-connect/token");
    //then make the request usin g QNetwork Library
    QNetworkRequest request(url);
    //add arguments/headers to the requesr
    request.setHeader(
        QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded"
    );

    //create a data object
    QurlQuery data;
    //add query items / arguments to the data object (required by opensky)
    data.addQueryItem("grant_type", "client_credentials");
    data.addQueryItem("client_id", clientId);
    data.addQueryItem("client_secrt", clientSecret);
    //post the request via the manager pointer created to the url.
    manager->post(request,data.toString(QUrl::FullyEncoded).toUtf8());
}
void OpenSkyApiHelper::replyFinished(QNetworkReply *reply){
    if (reply->error() != QNetworkReply::NoError) {
        //handle error here, if it fails we don't want ot parse it.
        emit networkErrorOccurred(reply->errorString());
        return;
    }
    //step 2 is to read the response into a Qbyte array
    QByteArray data = reply->readAll();    

    //next is parsing the response to extract the token.
    //after researching i found that Qt already has a JSON parser, safer to use thin in substr() as JSON formatting can change.
    QJsonDocument document = QJson document::fromJson(data);
    QJsonObject object = document.object();

    //quick check before storing a token, in case it's empty
    if (!object.contains("access_token") || !object["access_token"].isString()) {
        emit networkErrorOccurred("Access token missing from response");
        return;
}
    
    //finally, store access token- 
    accessToken = object["access_token"].toString();
    return; 
}

void OpenSkyApiHelper::handleNetworkError(QNetworkReply  *reply){

}
void OpenSkyApiHelper::parseFlightData(const QByteArray &data){

}


