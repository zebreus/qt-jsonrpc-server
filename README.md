# qt-jsonrpc-server
A header-only jsonrpc server for qt.

## Features
Call your QObjects using jsonrpc over websockets.

## Usage
```cpp
JsonRpcServer<YourService> server(80);
server.startListening();
```