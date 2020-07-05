#ifndef RESPONSE_H
#define RESPONSE_H

#include "message.h"
#include <QJsonObject>
#include <QJsonValue>

namespace jsonrpc{

class Response : public Message {

 public:

    Response(const QJsonObject& message);

    QJsonValue getResult() const;

    QJsonObject toJson() const override;

    Response(const QJsonValue& result);

    Response(const QJsonValue& id, const QJsonValue& result);

 private:
    QJsonValue result;
};

}

#endif // RESPONSE_H
