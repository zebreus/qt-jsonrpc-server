#ifndef DOUBLEARGUMENT_H
#define DOUBLEARGUMENT_H

#include "argument.h"
#include <cmath>

class DoubleArgument : public Argument
{
public:
    DoubleArgument(const QJsonValue& value);
private:
   QSharedPointer<double> value;
};

#endif // DOUBLEARGUMENT_H
