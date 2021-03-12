# qt-jsonrpc-server
The easiest way to create jsonrpc services in qt. Simply expose your QObjects as services

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