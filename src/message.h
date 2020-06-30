#ifndef MESSAGE_H
#define MESSAGE_H

#include <QVariant>
#include <QJsonObject>

class Message {

 public:

    Message(const QJsonObject& message);

    virtual QJsonValue getId() const;

    virtual bool hasId() const;

 private:
    QJsonValue id;
};

#endif // MESSAGE_H
