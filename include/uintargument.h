#ifndef UINTARGUMENT_H
#define UINTARGUMENT_H

#include "argument.h"
#include <cmath>

class UIntArgument : public Argument {

 public:

    UIntArgument(const QJsonValue& value);

 private:
    QSharedPointer<unsigned int> value;
};

#endif // UINTARGUMENT_H
