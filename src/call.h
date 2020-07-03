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

namespace jsonrpc{

class Call : public QObject{
    Q_OBJECT
public:
    Call(const Request& request, QObject* target, QObject *parent = nullptr);
    void invoke();

signals:
    void onError(Error error);
    void onSuccess(Response result);

private:
    bool setMethod(const QString& methodName);
    bool setArguments(const QList<QJsonValue>& providedArguments);
    QList<QMetaType::Type> getRequiredParameterTypes() const;
    bool addArgument(const QMetaType::Type& requiredType, const QJsonValue& providedParameter);

private:
    QList<QSharedPointer<Argument>> arguments;
    QMetaMethod method;
    QObject* processor;
    QJsonValue callId;

};

}

#endif // CALL_H
