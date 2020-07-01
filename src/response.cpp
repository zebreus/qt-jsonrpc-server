#include "response.h"

namespace jsonrpc{

Response::Response(const QJsonObject& message): Message(message)
{
    result = message.value("result");
    if(result.isUndefined()){
        throw "Invalid result";
    }
}

QJsonValue Response::getResult() const
{
    return result;
}

QJsonObject Response::toJson() const
{
    QJsonObject resultMessageJson = Message::toJson();
    resultMessageJson.insert("result",result);
    return resultMessageJson;
}

Response::Response(const QJsonValue &result):
    Message(), result(result)
{
    if(this->result.isUndefined()){
        this->result = QJsonValue(QJsonValue::Null);
    }
}

Response::Response(const QJsonValue &id, const QJsonValue &result):
    Message(id), result(result)
{
    if(this->result.isUndefined()){
        this->result = QJsonValue(QJsonValue::Null);
    }
}

}
