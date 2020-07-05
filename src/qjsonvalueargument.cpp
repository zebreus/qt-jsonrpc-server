#include "qjsonvalueargument.h"

QJsonValueArgument::QJsonValueArgument(const QJsonValue &providedArgument)
{
    QJsonValue* valuePointer = new QJsonValue(providedArgument);
    value.reset(valuePointer);
    argument = Q_ARG(QJsonValue, *valuePointer);
}
