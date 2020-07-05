#ifndef QJSONVALUEARGUMENT_H
#define QJSONVALUEARGUMENT_H

#include "argument.h"
#include <QJsonValue>

class QJsonValueArgument : public Argument
{
public:
    QJsonValueArgument(const QJsonValue& providedArgument);
private:
    QSharedPointer<QJsonValue> value;
};

#endif // QJSONVALUEARGUMENT_H
