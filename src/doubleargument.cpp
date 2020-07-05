#include "doubleargument.h"

DoubleArgument::DoubleArgument(const QJsonValue &providedArgument)
{
    if( providedArgument.isDouble() ){
        double* valuePointer = new double;
        *valuePointer = providedArgument.toDouble();
        value.reset(valuePointer);
        argument = Q_ARG(double, *valuePointer);
    }else{
        qDebug() << "Parameter type of parameter " << providedArgument.type() << " cannot be converted to double";
        throw "Error";
    }
}
