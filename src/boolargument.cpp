#include "boolargument.h"

BoolArgument::BoolArgument(const QJsonValue& providedArgument)
{
    if( providedArgument.isBool() ){
        bool* valuePointer = new bool;
        *valuePointer = providedArgument.toBool();
        value.reset(valuePointer);
        argument = Q_ARG(bool, *valuePointer);
    }else{
        qDebug() << "Parameter type of parameter " << providedArgument.type() << " cannot be converted to bool";
        throw "Error";
    }
}
