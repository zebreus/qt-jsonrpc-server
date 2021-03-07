#ifndef RESPONSE_H
#define RESPONSE_H

#include <QJsonObject>
#include <QJsonValue>

#include "message.h"

namespace jsonrpc {

class Response: public Message {
 public:
  Response(const QJsonObject& message);

  QJsonValue getResult() const;

  QJsonObject toJson() const override;

  Response(const QJsonValue& result);

  Response(const QJsonValue& id, const QJsonValue& result);

 private:
  QJsonValue result;
};

}  // namespace jsonrpc

#endif  // RESPONSE_H
