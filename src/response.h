#ifndef RESPONSE_H
#define RESPONSE_H

#include "message.h"
#include <QJsonObject>
#include <QJsonValue>

class Response : public Message {

 public:

    Response(const QJsonObject& message);

    QJsonValue getResult() const;

 public:
    QJsonValue result;
};

#endif // RESPONSE_H
