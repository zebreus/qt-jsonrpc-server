#ifndef REQUEST_H
#define REQUEST_H

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QList>
#include <QString>

#include "message.h"

namespace jsonrpc {

class Request: public Message {
 public:
  Request(const QJsonObject& message);

  QString getMethodName() const;

  QList<QJsonValue> getArguments() const;

  QJsonObject toJson() const override;

  Request(const QString& methodName, const QList<QJsonValue>& arguments);

  Request(const QJsonValue& id, const QString& methodName, const QList<QJsonValue>& arguments);

 private:
  QString methodName;
  QList<QJsonValue> arguments;

 private:
  bool buildArguments(const QJsonValue& passedArguments);
};

}  // namespace jsonrpc

#endif  // REQUEST_H
