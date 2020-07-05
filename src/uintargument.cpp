#include "uintargument.h"

UIntArgument::UIntArgument(const QJsonValue& providedArgument)
{
    if( providedArgument.isDouble() ){
        double doubleValue = providedArgument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            qDebug() << "Cannot convert double to unsigned integer, because it is not integer";
            throw "Error";
        }
        if (integerPart > (double)UINT_MAX || integerPart < (double)0){
            qDebug() << "Cannot convert double to unsigned integer, because it is out of range";
            throw "Error";
        }
        unsigned int* valuePointer = new unsigned int;
        *valuePointer = (unsigned int)integerPart;
        value.reset(valuePointer);
        argument = Q_ARG(unsigned int, *valuePointer);
    }else{
        qDebug() << "Parameter type of parameter " << providedArgument.type() << " cannot be converted to unsigned int";
        throw "Error";
    }
}
