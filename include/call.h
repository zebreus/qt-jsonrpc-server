#ifndef CALL_H
#define CALL_H

#include <QObject>
#include <QMetaMethod>
#include "request.h"
#include "response.h"
#include "error.h"
#include "argument.h"
#include "boolargument.h"
#include "intargument.h"
#include "uintargument.h"
#include "doubleargument.h"
#include "qcharargument.h"
#include "qstringargument.h"
#include "qjsonvalueargument.h"

namespace jsonrpc{

class Call : public QObject{
    Q_OBJECT
public:
    Call(const QSharedPointer<Request>& request, QObject* target, QObject *parent = nullptr);
    void invoke();

signals:
    void onError(QSharedPointer<Error> error);
    void onSuccess(QSharedPointer<Response> result);

private:
    QList<QMetaType::Type> getRequiredParameterTypes() const;

    //These methods will throw an Error object containing a error message if there is an error
    void setMethod(const QString& methodName);
    void setArguments(const QList<QJsonValue>& providedArguments);
    void addArgument(const QMetaType::Type& requiredType, const QJsonValue& providedParameter);

private:
    QList<QSharedPointer<Argument>> arguments;
    QMetaMethod method;
    QObject* processor;
    QJsonValue callId;

};

}

#endif // CALL_H
