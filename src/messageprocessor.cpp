#include "messageprocessor.h"

jsonrpc::MessageProcessor::MessageProcessor(QObject* parent): QObject(parent) {}

void jsonrpc::MessageProcessor::processIncomingDocument(const QJsonDocument& jsonDocument) {
  // Decide if message is a batch and handle
  if(jsonDocument.isObject()) {
    // Single call
    QJsonObject jsonObj = jsonDocument.object();
    processIncomingObject(jsonObj);
  } else if(jsonDocument.isArray()) {
    // TODO Implement proper batch calls
    // Batch call
    QJsonArray jsonArray = jsonDocument.array();
    for(const QJsonValue& jsonRpcObject : jsonArray) {
      if(jsonRpcObject.isObject()) {
        processIncomingObject(jsonRpcObject.toObject());
      } else {
        exceptions::InvalidRequest exception("your batch request contained something, that was not a json object.");
        sendMessage(QSharedPointer<Error>::create(exception.generateError()));
      }
    }
  } else if(jsonDocument.isEmpty()) {
    // Empty document, doing nothing
    return;
  } else {
    // Document is null,
    exceptions::InvalidMessage exception("your message is null.");
    sendMessage(QSharedPointer<Error>::create(exception.generateError()));
  }
}

void jsonrpc::MessageProcessor::processIncomingObject(const QJsonObject& message) {
  try {
    if(!message.value("method").isUndefined()) {
      Request* request = new Request(message);
      emit receivedRequest(QSharedPointer<Request>(request));
    } else if(!message.value("error").isUndefined()) {
      Error* error = new Error(message);
      emit receivedError(QSharedPointer<Error>(error));
    } else if(!message.value("result").isUndefined()) {
      Response* response = new Response(message);
      emit receivedResponse(QSharedPointer<Response>(response));
    } else {
      throw exceptions::InvalidMessage("Message is no request, response or error.");
    }
  } catch(const exceptions::JsonrpcException& exception) {
    sendMessage(QSharedPointer<Error>::create(exception.generateError(message.value("id"))));
  }
}

void jsonrpc::MessageProcessor::receiveMessage(const QString& message) {
  if(message.isEmpty()) {
    return;
  }
  QJsonParseError err;
  QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8(), &err);
  if(err.error == QJsonParseError::NoError) {
    processIncomingDocument(jsonDocument);
  } else {
    exceptions::ParseError exception("it has a " + err.errorString() + ".");
    sendMessage(QSharedPointer<Error>::create(exception.generateError()));
  }
}

void jsonrpc::MessageProcessor::sendMessage(const QSharedPointer<jsonrpc::Message>& message) {
  QJsonObject jsonObject = message->toJson();
  QJsonDocument document(jsonObject);
  emit outgoingMessage(document.toJson(QJsonDocument::Compact));
}
