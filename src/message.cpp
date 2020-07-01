#include "message.h"

namespace jsonrpc{

Message::Message(const QJsonObject& message)
{
    if(message.value("jsonrpc") != "2.0"){
        throw "Invalid message";
    }

    id = message.value("id");
    if( !(id.isUndefined() || id.isNull() || id.isString() || id.isDouble()) ){
        throw "Invalid message";
    }
}

QJsonValue Message::getId() const
{
    return id;
}

bool Message::hasId() const
{
    return !(id.isUndefined() || id.isNull());
}

QJsonObject Message::toJson() const
{
    QJsonObject jsonMessage;
    jsonMessage.insert("jsonrpc","2.0");
    jsonMessage.insert("id",id);
    return jsonMessage;
}

Message::Message()
{
    QString uuid = QUuid::createUuid().toString(QUuid::Id128);
    id = QJsonValue(uuid);
}

Message::Message(const QJsonValue& id): id(id)
{
    //Set id to undefined if it has the wrong type
    if( !(id.isUndefined() || id.isNull() || id.isString() || id.isDouble()) ){
        this->id = QJsonValue(QJsonValue::Undefined);
    }
}

}
