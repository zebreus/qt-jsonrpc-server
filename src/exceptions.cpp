#include <exceptions.h>

using namespace jsonrpc;

exceptions::JsonrpcException::JsonrpcException(const QString& message): message(message) {}

exceptions::JsonrpcException::~JsonrpcException() {}

Error exceptions::JsonrpcException::generateError(const QJsonValue& id) const {
  return Error(id, Error::InternalError, message);
}

QString exceptions::JsonrpcException::getMessage() {
  return message;
}

exceptions::JsonrpcException::JsonrpcException() {}

exceptions::InvalidSignature::InvalidSignature(const QString& message): JsonrpcException(message) {}

void exceptions::InvalidSignature::raise() const {
  throw *this;
}

exceptions::InvalidSignature* exceptions::InvalidSignature::clone() const {
  return new InvalidSignature(*this);
}

Error exceptions::InvalidSignature::generateError(const QJsonValue& id) const {
  return Error(id, Error::MethodNotFound, message);
}

exceptions::InvalidSignature::InvalidSignature() {}

exceptions::UnknownMethodName::UnknownMethodName(const QString& name) {
  message = "Method " + name + " not found.";
}

void exceptions::UnknownMethodName::raise() const {
  throw *this;
}

exceptions::UnknownMethodName* exceptions::UnknownMethodName::clone() const {
  return new UnknownMethodName(*this);
}

Error exceptions::UnknownMethodName::generateError(const QJsonValue& id) const {
  return Error(id, Error::MethodNotFound, message);
}

exceptions::WrongArgumentCount::WrongArgumentCount(int expected, int delivered, const QString& name) {
  message = "Method " + name + " expected " + QString::number(expected) + " arguments, but got " + QString::number(delivered) + ".";
}

exceptions::WrongArgumentCount::WrongArgumentCount(int expected, int delivered) {
  message = QString("Method expected ") + QString::number(expected) + " arguments, but got " + QString::number(delivered) + ".";
}

void exceptions::WrongArgumentCount::raise() const {
  throw *this;
}

exceptions::WrongArgumentCount* exceptions::WrongArgumentCount::clone() const {
  return new WrongArgumentCount(*this);
}

Error exceptions::WrongArgumentCount::generateError(const QJsonValue& id) const {
  return Error(id, Error::MethodNotFound, message);
}

exceptions::WrongArgumentType::WrongArgumentType(const QString& expectedType, const QString& receivedType, const QString& reason) {
  setMessage(expectedType, receivedType, reason);
}

exceptions::WrongArgumentType::WrongArgumentType(const QString& expectedType, const QJsonValue& receivedValue, const QString& reason) {
  QString receivedType;
  switch(receivedValue.type()) {
    case QJsonValue::Type::Null:
      receivedType = "null";
      break;
    case QJsonValue::Type::Bool:
      receivedType = "bool";
      break;
    case QJsonValue::Type::Double:
      receivedType = "number";
      break;
    case QJsonValue::Type::String:
      receivedType = "string";
      break;
    case QJsonValue::Type::Array:
      receivedType = QString("array (size ") + QString::number(receivedValue.toArray().size()) + ")";
      break;
    case QJsonValue::Type::Object:
      receivedType = "object";
      break;
    case QJsonValue::Type::Undefined:
      receivedType = "undefined";
      break;
  }
  setMessage(expectedType, receivedType, reason);
}

void exceptions::WrongArgumentType::setMessage(const QString& expectedType, const QString& receivedType, const QString& reason) {
  message = "Cannot convert " + receivedType + " to " + expectedType;
  if(reason.isEmpty()) {
    message += ".";
  } else {
    message += ", because " + reason;
  }
}

void exceptions::WrongArgumentType::raise() const {
  throw *this;
}

exceptions::WrongArgumentType* exceptions::WrongArgumentType::clone() const {
  return new WrongArgumentType(*this);
}

Error exceptions::WrongArgumentType::generateError(const QJsonValue& id) const {
  return Error(id, Error::InvalidParams, message);
}

exceptions::InvokationFailed::InvokationFailed(const QString& message): JsonrpcException(message) {}

void exceptions::InvokationFailed::raise() const {
  throw *this;
}

exceptions::InvokationFailed* exceptions::InvokationFailed::clone() const {
  return new InvokationFailed(*this);
}

Error exceptions::InvokationFailed::generateError(const QJsonValue& id) const {
  return Error(id, Error::InternalError, message);
}

exceptions::ParseError::ParseError(): JsonrpcException("Invalid JSON.") {}

void exceptions::ParseError::raise() const {
  throw *this;
}

exceptions::ParseError* exceptions::ParseError::clone() const {
  return new ParseError(*this);
}

Error exceptions::ParseError::generateError(const QJsonValue&) const {
  return generateError();
}

Error exceptions::ParseError::generateError() const {
  return Error(Error::ParseError, message);
}

exceptions::InvalidMessage::InvalidMessage(const QString& message): JsonrpcException(message) {}

void exceptions::InvalidMessage::raise() const {
  throw *this;
}

exceptions::InvalidMessage* exceptions::InvalidMessage::clone() const {
  return new InvalidMessage(*this);
}

Error exceptions::InvalidMessage::generateError(const QJsonValue&) const {
  return generateError();
}

Error exceptions::InvalidMessage::generateError() const {
  return Error(Error::InvalidMessage, message);
}

exceptions::InvalidMessage::InvalidMessage(): JsonrpcException() {}

exceptions::InvalidRequest::InvalidRequest(): InvalidMessage() {
  message = "The JSON sent is not a valid request object.";
}

void exceptions::InvalidRequest::raise() const {
  throw *this;
}

exceptions::InvalidRequest* exceptions::InvalidRequest::clone() const {
  return new InvalidRequest(*this);
}

Error exceptions::InvalidRequest::generateError(const QJsonValue&) const {
  return generateError();
}

Error exceptions::InvalidRequest::generateError() const {
  return Error(Error::InvalidRequest, message);
}

exceptions::InvalidResponse::InvalidResponse(): InvalidMessage() {
  message = "The JSON sent is not a valid response object.";
}

void exceptions::InvalidResponse::raise() const {
  throw *this;
}

exceptions::InvalidResponse* exceptions::InvalidResponse::clone() const {
  return new InvalidResponse(*this);
}

Error exceptions::InvalidResponse::generateError(const QJsonValue&) const {
  return generateError();
}

Error exceptions::InvalidResponse::generateError() const {
  return Error(Error::InvalidMessage, message);
}

exceptions::InvalidError::InvalidError(): InvalidMessage() {
  message = "The JSON sent is not a valid error object.";
}

void exceptions::InvalidError::raise() const {
  throw *this;
}

exceptions::InvalidError* exceptions::InvalidError::clone() const {
  return new InvalidError(*this);
}

Error exceptions::InvalidError::generateError(const QJsonValue&) const {
  return generateError();
}

Error exceptions::InvalidError::generateError() const {
  return Error(Error::InvalidMessage, message);
}
