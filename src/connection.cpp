#include "connection.h"

jsonrpc::Connection::Connection(QWebSocket* webSocket, QObject* target, QObject* parent)
    : QObject(parent), processor(target), webSocket(webSocket) {
  connect(webSocket, &QWebSocket::disconnected, this, &Connection::disconnect);

  messageProcessor = new MessageProcessor(this);
  connect(webSocket, &QWebSocket::textMessageReceived, messageProcessor, &MessageProcessor::receiveMessage);
  connect(webSocket, &QWebSocket::binaryMessageReceived, messageProcessor, &MessageProcessor::receiveMessage);
  connect(messageProcessor, &MessageProcessor::outgoingMessage, this, [webSocket](QString message) {
    webSocket->sendBinaryMessage(message.toUtf8());
  });

  callManager = new CallManager(target, this);
  connect(messageProcessor, &MessageProcessor::receivedRequest, callManager, &CallManager::processRequest);
  connect(callManager, &CallManager::respond, messageProcessor, &MessageProcessor::sendMessage);
}

jsonrpc::Connection::~Connection() {}

void jsonrpc::Connection::disconnect() {
  qDebug() << "disconnected";
  // TODO something
  deleteLater();
}
