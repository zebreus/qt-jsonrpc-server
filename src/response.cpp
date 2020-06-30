#include "response.h"

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
