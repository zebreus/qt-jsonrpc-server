# Thoughts about client interfaces
It would be useful to have a jsonrpc client library, that supports signals and offers the same ease of use as the server library. This document contains some ideas about how that could be realized.

## Client concepts
Currently there are 3 different ideas how a client could be implemented. Non of them is realized yet.

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