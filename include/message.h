#ifndef MESSAGE_H
#define MESSAGE_H

#include <QJsonObject>
#include <QUuid>
#include <QVariant>

namespace jsonrpc {

class Message {
 public:
  Message(const QJsonObject& message);

  virtual ~Message();

  virtual QJsonValue getId() const;

  virtual bool hasId() const;

  virtual QJsonObject toJson() const;

 protected:
  Message();

  Message(const QJsonValue& id);

 private:
  QJsonValue id;
};

}  // namespace jsonrpc

#endif  // MESSAGE_H
