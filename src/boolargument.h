#ifndef BOOLARGUMENT_H
#define BOOLARGUMENT_H

#include "argument.h"

class BoolArgument : public Argument {

 public:

    BoolArgument(const QJsonValue& value);

 private:
    QSharedPointer<bool> value;
};

#endif // BOOLARGUMENT_H
