# qt-jsonrpc-server
The easiest way to create jsonrpc services in qt. Simply expose your QObjects as services.

## Features
Call your QObjects using jsonrpc over websockets.

## Server usage
To create a service that offers a method to add two numbers, simply create a QObject with that method as a slot.
```cpp
class MyService : QObject{
    Q_OBJECT
public slots:
    inline int addNumbers(int a, int b){
        return a+b;
    }
};
```

You can then simply start offering that service on port 80 like:

```cpp
#include <QCoreApplication>
#include <server.h>

int main(){
    QCoreApplication a();
    jsonrpc::Server<YourService> server(80);
    server.startListening();
    return a.exec()
}
```

This will construct a new instance of your QObject for every incoming connection, and enable the client to call all slots with jsonrpc.

### Constructor arguments

If you want to pass arguments to the constructor of your Object, you can use
```cpp
server.setConstructorArguments(YOUR ARGUMENTS);
```
to do that. You have to ensure, that the arguments still exist, when the object is constructed.

### Emitting signals on the server

Your service QObject can emit signals and pass them to the client. To use this, the client first has to confirm, that it is capable of receiving signals, by calling the special method `rpc.qt.activate`. After that all signals emitted by your Qobject will be sent as jsonrpc 2.0 notifications. For more details see the [jsonrpc extension](qt-jsonrpc-extension.md).

## Future client usage
Currently there are 3 different ideas how a client could be implemented.
### Static client
You define your service as a QObject in the client code.
```cpp
class MyService : QObject{
    Q_OBJECT
public slots:
    int addNumbers(int a, int b);
};
```

Then something is created, that probably inherits or wraps your service object, but implements the qt metacalls itself.
```cpp
#include <QCoreApplication>
#include <client.h>

int main(){
    QCoreApplication a();
    Client<MyService> client("ws://127.0.0.1:80");
    //Maybe only by name
    connect(something, &Something::signal, &client, &Client::addNumbers);
    return a.exec()
}
```

#### pro
 * Errors at compiletime, instead of runtime.
 * Should be possible to use normal connect functions
 * Could be possible to check compatibility with a service
#### contra
 * The client has a static service interface
 * It will be possible to call the slots with strange behaviour.

### Unchecked runtime client
You can basically connect to random signals and slots on a proxy QObject, and it works, if it works and if not then not.

#### pro
 * quite flexible
 * Not necessary to hardcode service interface in client
#### contra
 * Runtime errors
 * custom connect functions required
 * More runtime errors

### Checked runtime client
The `rpc.qt.describe` method is called, to get the interface of the service. Then a proxy QObject is created, that supports the defined slots and signals.

#### pro
 * quite flexible
 * Not necessary to hardcode service interface in client
 * Runtime errors when connecting, instead of when calling
#### contra
 * Runtime errors
 * custom connect functions probably required
 * Only compatible with this server

## Limitations
 * You can only transfer types registered with the qt metatype system.
 * You can not transfer pointers, or complex types.
 * Currently there are some unsupported types, but most basic types are supported (integers of all sizes, bool, float, double, QString, QChar, QDate, QList<QString>, QVariant and a few more).
 * Currently only Websockets are the only supported transport protocol.