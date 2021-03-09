#include "message.h"

#include <exceptions.h>

namespace jsonrpc {

Message::Message(const QJsonObject& message) {
  if(message.value("jsonrpc") != "2.0") {
    throw exceptions::InvalidMessage("Invalid Message");
  }

  id = message.value("id");
  if(!(id.isUndefined() || id.isNull() || id.isString() || id.isDouble())) {
    throw exceptions::InvalidMessage("Invalid Message");
  }
}

Message::~Message() {}

QJsonValue Message::getId() const {
  return id;
}

bool Message::hasId() const {
  return !(id.isUndefined());
}

QJsonObject Message::toJson() const {
  QJsonObject jsonMessage;
  jsonMessage.insert("jsonrpc", "2.0");
  if(id != QJsonValue::Undefined) {
    jsonMessage.insert("id", id);
  }
  return jsonMessage;
}

Message::Message(): id(QJsonValue::Undefined) {}

Message::Message(const QJsonValue& id): id(id) {
  // Set id to undefined if it has an invalid type
  if(!(id.isNull() || id.isString() || id.isDouble())) {
    this->id = QJsonValue(QJsonValue::Undefined);
  }
}

}  // namespace jsonrpc
