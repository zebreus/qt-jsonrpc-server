#include "connection.h"

jsonrpc::Connection::Connection(QWebSocket* webSocket, QObject* target, QObject* parent)
    : QObject(parent), processor(target), webSocket(webSocket) {
  messageProcessor = new MessageProcessor(target, this);
  connect(webSocket, &QWebSocket::textMessageReceived, this, &Connection::receiveIncomingMessage);
  connect(webSocket, &QWebSocket::disconnected, this, &Connection::disconnect);
  connect(messageProcessor, &MessageProcessor::outgoingMessage, this, &Connection::receiveOutgoingMessage);
  connect(this, &Connection::incomingMessage, messageProcessor, &MessageProcessor::processIncomingMessage);
}

void jsonrpc::Connection::receiveIncomingMessage(const QString& message) {
  emit incomingMessage(message);
}

void jsonrpc::Connection::receiveOutgoingMessage(const QString& message) {
  webSocket->sendBinaryMessage(message.toUtf8());
}

void jsonrpc::Connection::disconnect() {
  qDebug() << "disconnected";
  // TODO something
  deleteLater();
}
