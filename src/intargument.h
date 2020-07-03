#ifndef INTARGUMENT_H
#define INTARGUMENT_H

#include "argument.h"
#include <cmath>

class IntArgument : public Argument {

 public:

    IntArgument(const QJsonValue& value);

 private:
    QSharedPointer<int> value;
};

#endif // INTARGUMENT_H
