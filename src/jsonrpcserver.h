#ifndef JSONRPCSERVER_H
#define JSONRPCSERVER_H

#include <QObject>
#include <QList>
#include "jsonrpcconnection.h"
#include <iostream>
#include <QString>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

using namespace std;

//Intermediate class that the JsonRpcServer inherits from, as template classes cannot use the QObject macro
//All signals and slots have to be defined here
class IntermediateJsonRpcServer : public QObject {
    Q_OBJECT
public:
    explicit IntermediateJsonRpcServer(QObject *parent = nullptr): QObject(parent){}
public Q_SLOTS:
    virtual void onNewConnection() {}
public: signals:
    void testSignal();
};


//Only allow templates for classes that are based on QObject
//Empty class for everything not based on QObject
template<class T, bool = std::is_base_of<QObject, T>::value>
class JsonRpcServer : IntermediateJsonRpcServer {};


//Class for everything based on QObject
template<class T>
class JsonRpcServer<T, true> : public IntermediateJsonRpcServer
{
private:
    QWebSocketServer* webSocketServer;
    QList<JsonRpcConnection<T>*> connections;
public:
    explicit JsonRpcServer(int port, QObject *parent = nullptr);
    ~JsonRpcServer();
    void startListening();
    void onNewConnection() override;

};

// Template implementation

template<class T>
JsonRpcServer<T,true>::JsonRpcServer(int port, QObject *parent) : IntermediateJsonRpcServer(parent){
    webSocketServer = new QWebSocketServer(QStringLiteral("SERVERNAME"), QWebSocketServer::NonSecureMode, this);
    if (webSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server listening on " << QHostAddress::Any << ":" << port;
        connect(webSocketServer, &QWebSocketServer::newConnection,
            this, &JsonRpcServer<T>::onNewConnection);
    }else{
        qDebug() << "Error opening server on " << QHostAddress::Any << ":" << port;
    }
}

template<class T>
JsonRpcServer<T,true>::~JsonRpcServer(){
    webSocketServer->close();
}

template<class T>
void JsonRpcServer<T,true>::startListening(){
    emit testSignal();
    //onNewConnection();
}

template<class T>
void JsonRpcServer<T,true>::onNewConnection(){
    QWebSocket *webSocket = webSocketServer->nextPendingConnection();
    JsonRpcConnection<T>* connection = new JsonRpcConnection<T>(webSocket,this);
    connections.push_back(connection);
}

#endif // JSONRPCSERVER_H
