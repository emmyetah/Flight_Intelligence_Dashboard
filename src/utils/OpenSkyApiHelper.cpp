#include "OpenSkyApiHelper.h"


//Learning notes

//QT documentation says that you only need one QAccessManager for a whole project

OpenSkyApiHelper::OpenSkyApiHelper(QObject *parent) : QObject(parent) {
    manager = new QNetworkAccessManager(this);
    
    connect(manager, //whos the sender
    &QNetworkAccessManager::finished, //how will you know its done (signal)
    this, //whose object fucniton should be called (receiever) 
    &OpenSkyApiHelper::replyFinished); //what function needs to run when it's done

}
//QUrl creates a structured url form the string you put in it. The ->get is telling the manager fucniton to send the HTTP Request using GET
manager->get(QNetworkRequest(QUrl(""))); // this will return a QNetworkReply*

