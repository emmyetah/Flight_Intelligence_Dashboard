#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

//main objective of this header file is to make a simple way for my service to intercat with the api
//I'll be using QtNetwork as my client ot connect to the REST OpenSky API.

class OpenSkyApiHelper : public QObject {
    Q_OBJECT //declaring class as a Q object, giving it access to Q object classes & fucntions

public: 
    //constructor declaration
    OpenSkyApiHelper(QObject *parent = nullptr);

private: 
    //declaring the manager pointer. Rule of thumb, use pointers not refernces or copies 
    QNetworkAccessManager *manager;
    //processes the response e.g., parse, check for errors etc.
    void replyFinished(QNetworkReply *reply)
} 