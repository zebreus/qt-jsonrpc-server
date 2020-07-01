#include "response.h"

namespace jsonrpc{

Response::Response(const QJsonObject& message): Message(message)
{
    result = message.value("result");
    if(message.value("result") == QJsonValue::Undefined){
        throw "Invalid result";
    }
}

QJsonValue Response::getResult() const
{
    return result;
}

QJsonObject Response::toJson() const
{

}

Response::Response(const QJsonValue &result)
{

}

Response::Response(const QJsonValue &id, const QJsonValue &result)
{

}

}
