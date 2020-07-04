#ifndef JSONRPCSERVER_H
#define JSONRPCSERVER_H

#include <QObject>
#include <QList>
#include "connection.h"
#include <iostream>
#include <QString>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>

using namespace std;
namespace jsonrpc{

//Intermediate class that the JsonRpcServer inherits from, as template classes cannot use the QObject macro
//All signals and slots have to be defined here
class IntermediateServer : public QObject {
    Q_OBJECT
public:
    explicit IntermediateServer(QObject *parent = nullptr): QObject(parent){}
public Q_SLOTS:
    virtual void onNewConnection() {}
public: signals:
    void testSignal();
};


//Only allow templates for classes that are based on QObject
//Empty class for everything not based on QObject
template<class T, bool = std::is_base_of<QObject, T>::value>
class Server : IntermediateServer {};


//Class for everything based on QObject
template<class T>
class Server<T, true> : public IntermediateServer
{
private:
    QWebSocketServer* webSocketServer;
    QList<Connection*> connections;
public:
    explicit Server(int port, QObject *parent = nullptr);
    ~Server();
    void startListening();
    void onNewConnection() override;

};

// Template implementation

template<class T>
Server<T,true>::Server(int port, QObject *parent) : IntermediateServer(parent){
    webSocketServer = new QWebSocketServer(QStringLiteral("SERVERNAME"), QWebSocketServer::NonSecureMode, this);
    if (webSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "Server listening on " << QHostAddress::Any << ":" << port;
        connect(webSocketServer, &QWebSocketServer::newConnection,
                this, &Server<T>::onNewConnection);
    }else{
        qDebug() << "Error opening server on " << QHostAddress::Any << ":" << port;
    }
}

template<class T>
Server<T,true>::~Server(){
    webSocketServer->close();
}

template<class T>
void Server<T,true>::startListening(){
    emit testSignal();
    //onNewConnection();
}

template<class T>
void Server<T,true>::onNewConnection(){
    QWebSocket *webSocket = webSocketServer->nextPendingConnection();
    T* qObject = new T();
    Connection* connection = new Connection(webSocket,this);
    ((QObject*)qObject)->setParent(connection);
    connections.push_back(connection);
}

}

#endif // JSONRPCSERVER_H
