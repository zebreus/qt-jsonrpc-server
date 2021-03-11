#include "connection.h"

jsonrpc::Connection::Connection(QObject* target, QObject* parent): QObject(parent), processor(target) {
  messageProcessor = new MessageProcessor(this);
  connect(messageProcessor, &MessageProcessor::outgoingMessage, this, &Connection::sendMessage);

  callManager = new CallManager(target, this);
  connect(messageProcessor, &MessageProcessor::receivedRequest, this, &Connection::processRequest);
  connect(callManager, &CallManager::respond, messageProcessor, &MessageProcessor::sendMessage);
}

jsonrpc::Connection::~Connection() {}

void jsonrpc::Connection::disconnect() {
  qDebug() << "disconnected";
  // TODO something
  deleteLater();
}

void jsonrpc::Connection::receiveMessage(const QString& message) {
  messageProcessor->receiveMessage(message);
}

void jsonrpc::Connection::processRequest(const QSharedPointer<jsonrpc::Request>& request) {
  const QString& methodName = request->getMethodName();
  if(!methodName.startsWith("rpc.")) {
    callManager->processRequest(request);
    return;
  }

  if(methodName == "rpc.qt.activate") {
    activateSignals();
    messageProcessor->sendMessage(QSharedPointer<Response>(new Response(request->getId(), QJsonValue(true))));
    return;
  }
  if(methodName == "rpc.qt.deactivate") {
    deactivateSignals();
    messageProcessor->sendMessage(QSharedPointer<Response>(new Response(request->getId(), QJsonValue(true))));
    return;
  }
  if(methodName == "rpc.qt.describe") {
    QJsonValue description = describeInterface();
    messageProcessor->sendMessage(QSharedPointer<Response>(new Response(request->getId(), QJsonValue(description))));
    return;
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

QJsonValue jsonrpc::Connection::describeInterface() {
  InterfaceDescription description(*processor->metaObject());
  ArgumentImplementation<InterfaceDescription> descriptionArgument(&description);
  return descriptionArgument.getJson();
}
