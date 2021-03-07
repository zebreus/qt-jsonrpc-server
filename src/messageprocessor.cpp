#include "messageprocessor.h"

jsonrpc::MessageProcessor::MessageProcessor(QObject* processor, QObject* parent): QObject(parent), processor(processor) {
  requestManager = new RequestManager(processor, this);
  callManager = new CallManager(processor, this);
  connect(callManager, &CallManager::respond, this, &MessageProcessor::processOutgoingMessage);
  connect(requestManager, &RequestManager::sendingRequest, this, &MessageProcessor::processOutgoingMessage);
}

void jsonrpc::MessageProcessor::processIncomingDocument(const QJsonDocument& jsonDocument) {
  // Decide if message is a batch and handle
  if(jsonDocument.isObject()) {
    // Single call
    QJsonObject jsonObj = jsonDocument.object();
    processIncomingObject(jsonObj);
  } else if(jsonDocument.isArray()) {
    // Batch call
    QJsonArray jsonArray = jsonDocument.array();
    for(QJsonValue jsonRpcObject : jsonArray) {
      if(jsonRpcObject.isObject()) {
        processIncomingObject(jsonRpcObject.toObject());
      } else {
        QString errorMessage = "Entry of batch request is no json object";
        Error* error = new Error(QJsonValue::Null, Error::Code::InvalidRequest, errorMessage);
        processOutgoingMessage(QSharedPointer<Error>(error));
      }
    }
  } else if(jsonDocument.isEmpty()) {
    // Empty document, doing nothing
  } else {
    // Document is null, probably some kind of parsing error, should not happen
    QString errorMessage = "Request is null";
    Error* error = new Error(QJsonValue::Null, Error::Code::ParseError, errorMessage);
    processOutgoingMessage(QSharedPointer<Error>(error));
  }
}

void jsonrpc::MessageProcessor::processIncomingObject(const QJsonObject& message) {
  if(!message.value("method").isUndefined()) {
    try {
      Request* request = new Request(message);
      processIncomingRequest(QSharedPointer<Request>(request));
    } catch(const QString& errorMessage) {
      Error* error = new Error(message.value("id"), Error::Code::InvalidRequest, errorMessage);
      processOutgoingMessage(QSharedPointer<Error>(error));
    }
  } else if(!message.value("error").isUndefined()) {
    try {
      Error* error = new Error(message);
      processIncomingError(QSharedPointer<Error>(error));
    } catch(const QString& errorMessage) {
      Error* error = new Error(message.value("id"), Error::Code::InvalidRequest, errorMessage);
      processOutgoingMessage(QSharedPointer<Error>(error));
    }
  } else if(!message.value("result").isUndefined()) {
    try {
      Response* response = new Response(message);
      processIncomingResponse(QSharedPointer<Response>(response));
    } catch(const QString& errorMessage) {
      Error* error = new Error(message.value("id"), Error::Code::InvalidRequest, errorMessage);
      processOutgoingMessage(QSharedPointer<Error>(error));
    }
  } else {
    QString errorMessage = "Message is no request response or error";
    Error* error = new Error(message.value("id"), Error::Code::InvalidRequest, errorMessage);
    processOutgoingMessage(QSharedPointer<Error>(error));
  }
}

void jsonrpc::MessageProcessor::processIncomingRequest(const QSharedPointer<jsonrpc::Request>& request) {
  callManager->processRequest(request);
}

void jsonrpc::MessageProcessor::processIncomingResponse(const QSharedPointer<jsonrpc::Response>& response) {
  requestManager->processResponse(response);
}

void jsonrpc::MessageProcessor::processIncomingError(const QSharedPointer<jsonrpc::Error>& error) {
  requestManager->processError(error);
}

void jsonrpc::MessageProcessor::processIncomingMessage(const QString& message) {
  QJsonParseError err;
  QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8(), &err);
  if(err.error == QJsonParseError::NoError) {
    processIncomingDocument(jsonDocument);
  } else {
    qDebug() << "QJsonParseError while processing message: " << message;
    QString errorMessage = "Error parsing json";
    QJsonValue detailedErrorMessage = err.errorString();
    Error* error = new Error(QJsonValue::Null, Error::Code::ParseError, errorMessage, detailedErrorMessage);
    processOutgoingMessage(QSharedPointer<Error>(error));
  }
}

void jsonrpc::MessageProcessor::processOutgoingMessage(const QSharedPointer<jsonrpc::Message>& message) {
  QJsonObject jsonObject = message->toJson();
  QJsonDocument document(jsonObject);
  emit outgoingMessage(document.toJson(QJsonDocument::Compact));
}
