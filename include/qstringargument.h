#ifndef QSTRINGARGUMENT_H
#define QSTRINGARGUMENT_H

#include "argument.h"
#include <QString>

class QStringArgument : public Argument
{
public:
    QStringArgument(const QJsonValue& providedArgument);
private:
   QSharedPointer<QString> value;
};

#endif // QSTRINGARGUMENT_H
