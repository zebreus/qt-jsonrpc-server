#ifndef MESSAGE_H
#define MESSAGE_H

#include <QVariant>
#include <QJsonObject>
#include <QUuid>

namespace jsonrpc{

class Message {

 public:

    Message(const QJsonObject& message);

    virtual QJsonValue getId() const;

    virtual bool hasId() const;

    virtual QJsonObject toJson() const;

protected:

    Message();

    Message(const QJsonValue& id);

 private:
    QJsonValue id;
};

}

#endif // MESSAGE_H
