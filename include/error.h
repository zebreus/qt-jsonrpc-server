#ifndef ERROR_H
#define ERROR_H

#include <QJsonObject>
#include <QJsonValue>
#include <QString>

#include "message.h"

namespace jsonrpc {

class Error: public Message {
 public:
  enum Code { ParseError = -32700, InvalidRequest = -32600, MethodNotFound = -32601, InvalidParams = -32602, InternalError = -32603 };

 public:
  Error(const QJsonObject& message);

  int getCode() const;

  QString getMessage() const;

  QJsonValue getData() const;

  bool hasData() const;

  QJsonObject toJson() const override;

  Error(int code, const QString& errorMessage, const QJsonValue& data);

  Error(int code, const QString& errorMessage);

  Error(const QJsonValue& id, int code, const QString& errorMessage, const QJsonValue& data);

  Error(const QJsonValue& id, int code, const QString& errorMessage);

 private:
  int code;
  QString message;
  QJsonValue data;
};

}  // namespace jsonrpc

#endif  // ERROR_H
