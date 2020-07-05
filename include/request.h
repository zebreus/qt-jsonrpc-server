#ifndef REQUEST_H
#define REQUEST_H

#include "message.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QList>
#include <QString>

namespace jsonrpc{

class Request : public Message {

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

}

#endif // REQUEST_H
