#ifndef QCHARARGUMENT_H
#define QCHARARGUMENT_H

#include "argument.h"
#include <QChar>
#include <QString>

class QCharArgument : public Argument
{
public:
    QCharArgument(const QJsonValue& providedArgument);
private:
   QSharedPointer<QChar> value;
};

#endif // QCHARARGUMENT_H
