#include "qstringargument.h"

QStringArgument::QStringArgument(const QJsonValue &providedArgument)
{
    if( providedArgument.isString() ){
        QString* valuePointer = new QString(providedArgument.toString());
        value.reset(valuePointer);
        argument = Q_ARG(QString, *valuePointer);
    }else{
        qDebug() << "Parameter type of parameter " << providedArgument.type() << " cannot be converted to QString";
        throw "Error";
    }
}
