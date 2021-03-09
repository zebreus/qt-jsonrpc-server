#include "error.h"

#include <exceptions.h>

namespace jsonrpc {

Error::Error(const QJsonObject& message): Message(message) {
  QJsonValue jsonError = message.value("error");
  if(jsonError.isObject()) {
    QJsonValue jsonErrorCode = jsonError.toObject().value("code");
    if(jsonErrorCode.isDouble()) {
      code = jsonErrorCode.toInt();
    } else {
      throw exceptions::InvalidError();
    }

    QJsonValue jsonErrorMessage = jsonError.toObject().value("message");
    if(jsonErrorMessage.isString()) {
      this->message = jsonErrorMessage.toString();
    } else {
      throw exceptions::InvalidError();
    }

    QJsonValue data = jsonError.toObject().value("data");

  } else {
    throw exceptions::InvalidError();
  }
}

int Error::getCode() const {
  return code;
}

QString Error::getMessage() const {
  return message;
}

QJsonValue Error::getData() const {
  return data;
}

bool Error::hasData() const {
  return !data.isUndefined();
}

QJsonObject Error::toJson() const {
  QJsonObject errorObject;
  errorObject.insert("code", code);
  errorObject.insert("message", message);
  if(hasData()) {
    errorObject.insert("data", data);
  }

  QJsonObject errorMessageJson = Message::toJson();
  errorMessageJson.insert("error", errorObject);
  return errorMessageJson;
}

Error::Error(int code, const QString& errorMessage, const QJsonValue& data): Message(), code(code), message(errorMessage), data(data) {}

Error::Error(int code, const QString& errorMessage): Message(), code(code), message(errorMessage), data(QJsonValue::Undefined) {}

Error::Error(const QJsonValue& id, int code, const QString& errorMessage, const QJsonValue& data)
    : Message(id), code(code), message(errorMessage), data(data) {}

Error::Error(const QJsonValue& id, int code, const QString& errorMessage)
    : Message(id), code(code), message(errorMessage), data(QJsonValue::Undefined) {}

}  // namespace jsonrpc
