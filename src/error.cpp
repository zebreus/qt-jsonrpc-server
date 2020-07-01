#include "error.h"

namespace jsonrpc{

Error::Error(const QJsonObject& message): Message(message)
{
    QJsonValue jsonError = message.value("error");
    if(jsonError.isObject()){
        QJsonValue jsonErrorCode = jsonError.toObject().value("code");
        if(jsonErrorCode.isDouble()){
            code = jsonErrorCode.toInt();
        }else{
            throw "Invalid error";
        }

        QJsonValue jsonErrorMessage = jsonError.toObject().value("message");
        if(jsonErrorMessage.isString()){
            this->message = jsonErrorMessage.toString();
        }else{
            throw "Invalid error";
        }

        QJsonValue data = jsonError.toObject().value("data");

    }else{
        throw "Invalid error";
    }
}

int Error::getCode() const
{
    return code;
}

QString Error::getMessage() const
{
    return message;
}

QJsonValue Error::getData() const
{
    return data;
}

bool Error::hasData() const
{
    return !(data.isUndefined() || data.isNull());
}

QJsonObject Error::toJson() const
{

}

Error::Error(int code, const QString &errorMessage, const QJsonValue &data)
{

}

Error::Error(int code, const QString &errorMessage)
{

}

Error::Error(const QJsonValue &id, int code, const QString &errorMessage, const QJsonValue &data)
{

}

Error::Error(const QJsonValue &id, int code, const QString &errorMessage)
{

}

}
