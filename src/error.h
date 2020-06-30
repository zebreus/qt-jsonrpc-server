#ifndef ERROR_H
#define ERROR_H

#include "message.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QString>


class Error : public Message {

 public:

    Error(const QJsonObject& message);

    int getCode() const;

    QString getMessage() const;

    QJsonValue getData() const;

    bool hasData() const;

 private:
    int code;
    QString message;
    QJsonValue data;
};

#endif // ERROR_H
