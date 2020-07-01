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
    QJsonObject errorObject;
    errorObject.insert("code", code);
    errorObject.insert("message", message);
    //TODO check if just adding QJsonValue::Undefined has the same effect
    if(!data.isUndefined()){
        errorObject.insert("data",data);
    }

    QJsonObject errorMessageJson = Message::toJson();
    errorMessageJson.insert("error",errorObject);
    return errorMessageJson;
}

Error::Error(int code, const QString &errorMessage, const QJsonValue &data):
    Message(), code(code), message(errorMessage), data(data)
{

}

Error::Error(int code, const QString &errorMessage):
    Message(), code(code), message(errorMessage), data(QJsonValue::Undefined)
{

}

Error::Error(const QJsonValue &id, int code, const QString &errorMessage, const QJsonValue &data):
    Message(id), code(code), message(errorMessage), data(data)
{

}

Error::Error(const QJsonValue &id, int code, const QString &errorMessage):
    Message(id), code(code), message(errorMessage), data(QJsonValue::Undefined)
{

}

}
