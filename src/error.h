#ifndef ERROR_H
#define ERROR_H

#include "message.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

namespace jsonrpc{

class Error : public Message {

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

}

#endif // ERROR_H
