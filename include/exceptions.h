#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <error.h>

#include <QException>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

namespace jsonrpc {
namespace exceptions {

class JsonrpcException: public QException {
 public:
  JsonrpcException(const QString& message);
  virtual ~JsonrpcException();
  virtual void raise() const = 0;
  virtual JsonrpcException* clone() const = 0;
  virtual Error generateError(const QJsonValue& id) const;
  QString getMessage();

 protected:
  JsonrpcException();
  QString message;
};

class InvalidSignature: public JsonrpcException {
 public:
  InvalidSignature(const QString& message);
  void raise() const override;
  InvalidSignature* clone() const override;
  Error generateError(const QJsonValue& id) const override;

 protected:
  InvalidSignature();
};

class UnknownMethodName: public InvalidSignature {
 public:
  UnknownMethodName(const QString& name);
  void raise() const override;
  UnknownMethodName* clone() const override;
  Error generateError(const QJsonValue& id) const override;
};

class WrongArgumentCount: public InvalidSignature {
 public:
  WrongArgumentCount(int expected, int delivered, const QString& name);
  WrongArgumentCount(int expected, int delivered);
  void raise() const override;
  WrongArgumentCount* clone() const override;
  Error generateError(const QJsonValue& id) const override;
};

class WrongArgumentType: public InvalidSignature {
 public:
  WrongArgumentType(const QString& expectedType, const QString& receivedType, const QString& reason = "");
  WrongArgumentType(const QString& expectedType, const QJsonValue& receivedValue, const QString& reason = "");
  void raise() const override;
  WrongArgumentType* clone() const override;
  Error generateError(const QJsonValue& id) const override;

 private:
  void setMessage(const QString& expectedType, const QString& receivedType, const QString& reason = "");
};

class InvokationFailed: public JsonrpcException {
 public:
  InvokationFailed(const QString& message);
  void raise() const override;
  InvokationFailed* clone() const override;
  Error generateError(const QJsonValue& id) const override;
};

}  // namespace exceptions
}  // namespace jsonrpc

#endif
