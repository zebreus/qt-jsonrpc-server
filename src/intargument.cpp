#include "intargument.h"

IntArgument::IntArgument(const QJsonValue& providedArgument)
{
    if( providedArgument.isDouble() ){
        double doubleValue = providedArgument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            qDebug() << "Cannot convert double to integer, because it is not integer";
            throw "Error";
        }
        if (integerPart > (double)INT_MAX || integerPart < (double)INT_MIN){
            qDebug() << "Cannot convert double to integer, because it is out of range";
            throw "Error";
        }

        int* valuePointer = new int;
        *valuePointer = (int)integerPart;
        value.reset(valuePointer);
        argument = Q_ARG(int, *valuePointer);
    }else{
        qDebug() << "Parameter type of parameter " << providedArgument.type() << " cannot be converted to bool";
        throw "Error";
    }
}
