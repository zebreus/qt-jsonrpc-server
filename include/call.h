#ifndef CALL_H
#define CALL_H

#include <QObject>
#include <QMetaMethod>
#include "request.h"
#include "response.h"
#include "error.h"
#include "argument.h"
#include "exceptions.h"

namespace jsonrpc{

class Call : public QObject{
    Q_OBJECT
public:
    Call(QObject* target, const QString& method, const QList<QJsonValue>& arguments, QObject *parent = nullptr);
    void invoke();

signals:
    void onError(Error error);
    void onSuccess(QJsonValue result);

private:
    QList<int> getRequiredParameterTypes() const;

    //These methods will throw an Error object containing a error message if there is an error
    void setMethod(const QString& methodName);
    void setArguments(const QList<QJsonValue>& providedArguments);
    void addArgument(const int requiredTypeId, const QJsonValue& providedParameter);

private:
    QList<QSharedPointer<Argument>> arguments;
    QMetaMethod method;
    QObject* processor;

};

}

#endif // CALL_H
