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

void jsonrpc::Connection::processRequest(const QSharedPointer<jsonrpc::Request>& request) {
  const QString& methodName = request->getMethodName();
  if(!methodName.startsWith("rpc.")) {
    callManager->processRequest(request);
  }

  if(methodName == "rpc.qt.activate") {
    activateSignals();
    messageProcessor->sendMessage(QSharedPointer<Response>(new Response(request->getId(), QJsonValue(true))));
  } else if(methodName == "rpc.qt.deactivate") {
    deactivateSignals();
    messageProcessor->sendMessage(QSharedPointer<Response>(new Response(request->getId(), QJsonValue(true))));
  } else if(methodName == "rpc.qt.describe") {
    // describeInterface();
    exceptions::UnknownMethodName exception(methodName);
    messageProcessor->sendMessage(QSharedPointer<Error>(new Error(exception.generateError(request->getId()))));
  }

  exceptions::UnknownMethodName exception(methodName);
  messageProcessor->sendMessage(QSharedPointer<Error>(new Error(exception.generateError(request->getId()))));
}

void jsonrpc::Connection::activateSignals() {
  signalConverter = new SignalConverter(processor, this);
  connect(signalConverter, &SignalConverter::convertedSignal, messageProcessor, &MessageProcessor::sendMessage);
}

void jsonrpc::Connection::deactivateSignals() {
  if(signalConverter != nullptr) {
    QObject::disconnect(signalConverter, &SignalConverter::convertedSignal, messageProcessor, &MessageProcessor::sendMessage);
    delete signalConverter;
  }
}
