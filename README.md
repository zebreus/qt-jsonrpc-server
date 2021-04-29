# qt-jsonrpc-server
The easiest way to create jsonrpc services in Qt. Simply expose your QObjects as services.

## Features
Call slots and emit signals of QObjects via [JSON-RPC 2.0](https://www.jsonrpc.org/specification).

## Add qt-jsonrpc-server to your project
Clone this repository into your project folder and add the following line to the top of your qmake project file:
```qmake
include($$PWD/qt-jsonrpc-server/qt-jsonrpc-server.pri)
```

## Server usage
To create a service that offers a method to add two numbers, simply create a QObject based class with that methods as a slot.
Any QObject class can be offered as a jsonrpc service. Let's assume you want to offer a service that offers an increasable counter. You would just write a completely normal QObject class like this:
```cpp
class MyCounter : QObject{
    Q_OBJECT
    inline int counter = 0;
signals:
    void counterChanged(int counter);
public slots:
    inline void addToCounter(int value){
        counter += number;
        emit counterChanged(counter);
    }
    inline int getCounter(){
        return counter;
    }
};
```

### Starting the server
To offer the signals and slots of that class as a service, you just have to create and start a jsonrpc server for that class. This will start a websockets server on port 5673. A new instance of your QObject will be constructed for every incoming connection.
```cpp
...
#include <server.h>
#include "mycounter.h"

int main(){
    QCoreApplication a();

    jsonrpc::Server<MyCounter> server(5673);
    server.startListening();

    return a.exec()
}
```

You can now call the slots of your counter via jsonrpc.
<details>
<summary>Example</summary>

```console
$ websocat ws://localhost:5673
> {"jsonrpc": "2.0", "method": "getCounter", "params": [], "id": 1}
{"id":1,"jsonrpc":"2.0","result":0}
> {"jsonrpc": "2.0", "method": "addToCounter", "params": [23], "id": 2}
{"id":2,"jsonrpc":"2.0","result":null}
> {"jsonrpc": "2.0", "method": "getCounter", "params": [], "id": 3}
{"id":3,"jsonrpc":"2.0","result":23}
```
</details>

### Passing constructor arguments to the service
If you want to pass arguments to the constructor of your Object, you can use
```cpp
server.setConstructorArguments(YOUR ARGUMENTS);
```
to do that. **You have to ensure, that the arguments are still valid, when the object is constructed / for the lifetime of the server.**

### Emitting signals
All signals emitted by your service QObject will be passed to the client, if they requested that. To use this, the client first has to enable signals for the connection, by calling the special method `rpc.qt.activate()`. After that all signals emitted by your Qobject will be sent as jsonrpc 2.0 notifications. You can disable signals for that connection again, by calling the the special method `rpc.qt.deactivate()`. For more details see the [jsonrpc extension](qt-jsonrpc-extension.md).
<details>
<summary>Example</summary>

```console
$ websocat ws://localhost:5673
> {"jsonrpc": "2.0", "method": "addToCounter", "params": [1], "id": 2}
{"id":2,"jsonrpc":"2.0","result":null}
> {"jsonrpc": "2.0", "method": "rpc.qt.activate", "params": [], "id": 3}
{"id":3,"jsonrpc":"2.0","result":true}
> {"jsonrpc": "2.0", "method": "addToCounter", "params": [2], "id": 4}
{"jsonrpc":"2.0","method":"counterChanged","params":[3]}
{"id":4,"jsonrpc":"2.0","result":null}
> {"jsonrpc": "2.0", "method": "addToCounter", "params": [4]}
{"jsonrpc":"2.0","method":"counterChanged","params":[7]}
> {"jsonrpc": "2.0", "method": "rpc.qt.deactivate", "params": [], "id": 5}
{"id":5,"jsonrpc":"2.0","result":true}
> {"jsonrpc": "2.0", "method": "addToCounter", "params": [8], "id": 6}
{"id":6,"jsonrpc":"2.0","result":null}
> {"jsonrpc": "2.0", "method": "getCounter", "params": [], "id": 7}
{"id":7,"jsonrpc":"2.0","result":15}
```
</details>

### Getting information about the server interface
A client can request information about the server interface, by calling `rpc.qt.describe()`. For more details see the [jsonrpc extension](qt-jsonrpc-extension.md).
<details>
<summary>Example</summary>

Newlines added to response for readability

```console
$ websocat ws://localhost:5673
> {"jsonrpc": "2.0", "method": "rpc.qt.describe", "params": [], "id": 1}
```  ```json
{"id":1,"jsonrpc":"2.0","result":{
 "name":"MyCounter",
 "signals":[
  {"name":"counterChanged","parameters":["int"],"return":"void"}
 ],
 "slots":[
  {"name":"addToCounter","parameters":["int"],"return":"void"},
  {"name":"getCounter","parameters":[],"return":"int"}
 ],
 "version":0
}}
```
</details>

## Limitations
 * You can only transfer types registered with the qt metatype system.
 * You can not transfer pointers, or complex types.
 * Currently there are some unsupported types, but most basic types are supported (integers of all sizes, bool, float, double, QString, QChar, QDate, QList<QString>, QVariant and a few more).
 * Currently only Websockets are the only supported transport protocol.

## TODO
- [ ] Add option to use a single shared object for all connections.
- [ ] Add option to disable signals completely.
- [ ] Adjust `rpc.qt.describe()` method, to use the [openRPC interface description](https://spec.open-rpc.org/#introduction).
- [ ] Create a client. See [client.md](client.md).
- [ ] Add support for other transport protocols than websockets. Should be quite easy.
- [ ] Add some documentatin about the inner workings of the library.
- [ ] Remove serialization and deserialization from the Argument class.
- [ ] Make sure googletests work on all platforms.
- [ ] Add instructions on how to build/develop this library
- [ ] Remove outdated classdiagram