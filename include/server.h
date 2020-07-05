#ifndef JSONRPCSERVER_H
#define JSONRPCSERVER_H

#include <QObject>
#include <QList>
#include "connection.h"
#include <iostream>
#include <QString>
#include <QtWebSockets/QWebSocketServer>
#include <QtWebSockets/QWebSocket>
#include <functional>

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
    std::function<T*()> constructorFunction;
public:
    explicit Server(int port, QObject *parent = nullptr);
    ~Server();
    void startListening();
    void onNewConnection() override;

    template <typename ...Args>
    void setConstructorArguments(Args&&... args);

};

// Template implementation

template<class T>
Server<T,true>::Server(int port, QObject *parent) : IntermediateServer(parent){
    webSocketServer = new QWebSocketServer(QStringLiteral("SERVERNAME"), QWebSocketServer::NonSecureMode, this);
    setConstructorArguments();
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
}

template<class T>
void Server<T,true>::onNewConnection(){
    QWebSocket *webSocket = webSocketServer->nextPendingConnection();
    T* target = constructorFunction();
    Connection* connection = new Connection(webSocket,target,this);
    ((QObject*)target)->setParent(connection);
    connections.push_back(connection);
}

template <class T>
template <typename ...Args>
void Server<T,true>::setConstructorArguments(Args&&... args)
{
    //Create a tuple containing the arguments
    auto argsTuple = std::tuple<Args...>(args...);
    //Create a lambda that captures the argsTuple and uses it to create a new T
    std::function<T*()> constructorFunction = [argsTuple]()
    {
        T* result = std::apply([](auto &&... args) { return new T(args...); }, argsTuple);
        return result;
    };
    //Set the constructor function
    this->constructorFunction = constructorFunction;
}

}

#endif // JSONRPCSERVER_H
