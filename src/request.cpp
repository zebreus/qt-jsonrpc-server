#include "request.h"

namespace jsonrpc{

Request::Request(const QJsonObject& message): Message(message)
{
    QJsonValue jsonMethodName = message.value("method");
    if(!jsonMethodName.isString()){
        throw "Invalid request";
    }
    methodName = jsonMethodName.toString();

    if(!buildArguments(message.value("params"))){
        throw "Invalid request";
    }
}

QString Request::getMethodName() const
{
    return methodName;
}

QList<QJsonValue> Request::getArguments() const
{
    return arguments;
}

QJsonObject Request::toJson() const
{

}

Request::Request(const QString &methodName, const QList<QJsonValue> &arguments)
{

}

Request::Request(const QJsonValue &id, const QString &methodName, const QList<QJsonValue> &arguments)
{

}

bool Request::buildArguments(const QJsonValue& passedArguments)
{
    switch (passedArguments.type()){
    case QJsonValue::Array:
        for(QJsonValue val : passedArguments.toArray()){
            arguments.append(val);
        }
        break;
    case QJsonValue::Undefined:
        // It is valid to have no parameters
        break;
    case QJsonValue::Object:
        //TODO parameter passing by name is also valid, but not implemented yet
        qDebug() << "Parameter passing by name is not implemented.";
        return false;
    default:
        return false;
    }

    return true;
}

}
