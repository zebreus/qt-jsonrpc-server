#include "qcharargument.h"

QCharArgument::QCharArgument(const QJsonValue &providedArgument)
{
    if( providedArgument.isString() ){
        QString stringValue = providedArgument.toString();
        if(stringValue.length() != 1){
            qDebug() << "Cannot convert string to char, because it is not one char long";
            throw "Error";
        }
        QChar* valuePointer = new QChar(stringValue.front());
        value.reset(valuePointer);
        argument = Q_ARG(QChar, *valuePointer);
    }else{
        qDebug() << "Parameter type of parameter " << providedArgument.type() << " cannot be converted to char";
        throw "Error";
    }
}
