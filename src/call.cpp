#include "call.h"

jsonrpc::Call::Call(QObject* target, const QString& method, const QList<QJsonValue>& arguments, QObject *parent):
    QObject(parent), processor(target)
{

    //Throwing an Error object if an error is encountered during construction
    //TODO improve error handling
    //callId = request->getId();
    setMethod(method);
    setArguments(arguments);
}

void jsonrpc::Call::invoke()
{
    QMetaType::Type returnType = (QMetaType::Type)method.returnType();
    Argument* argument = Argument::create(returnType);
    QGenericReturnArgument returnArgument = QGenericReturnArgument(QMetaType::typeName(returnType), argument->getArgument().data());

    switch(arguments.size()){
    case 0:
        method.invoke(processor, Qt::AutoConnection, returnArgument);
        break;
    case 1:
        method.invoke(processor, Qt::AutoConnection, returnArgument, arguments[0]->getArgument());
        break;
    case 2:
        method.invoke(processor, Qt::AutoConnection, returnArgument, arguments[0]->getArgument(), arguments[1]->getArgument());
        break;
    case 3:
        method.invoke(processor, Qt::AutoConnection, returnArgument, arguments[0]->getArgument(), arguments[1]->getArgument(), arguments[2]->getArgument());
        break;
    case 4:
        method.invoke(processor, Qt::AutoConnection, returnArgument, arguments[0]->getArgument(), arguments[1]->getArgument(), arguments[2]->getArgument(), arguments[3]->getArgument());
        break;
    case 5:
        method.invoke(processor, Qt::AutoConnection, returnArgument, arguments[0]->getArgument(), arguments[1]->getArgument(), arguments[2]->getArgument(), arguments[3]->getArgument(), arguments[4]->getArgument());
        break;
    case 6:
        method.invoke(processor, Qt::AutoConnection, returnArgument, arguments[0]->getArgument(), arguments[1]->getArgument(), arguments[2]->getArgument(), arguments[3]->getArgument(), arguments[4]->getArgument(), arguments[5]->getArgument());
        break;
    case 7:
        method.invoke(processor, Qt::AutoConnection, returnArgument, arguments[0]->getArgument(), arguments[1]->getArgument(), arguments[2]->getArgument(), arguments[3]->getArgument(), arguments[4]->getArgument(), arguments[5]->getArgument(), arguments[6]->getArgument());
        break;
    case 8:
        method.invoke(processor, Qt::AutoConnection, returnArgument, arguments[0]->getArgument(), arguments[1]->getArgument(), arguments[2]->getArgument(), arguments[3]->getArgument(), arguments[4]->getArgument(), arguments[5]->getArgument(), arguments[6]->getArgument(), arguments[7]->getArgument());
        break;
    case 9:
        method.invoke(processor, Qt::AutoConnection, returnArgument, arguments[0]->getArgument(), arguments[1]->getArgument(), arguments[2]->getArgument(), arguments[3]->getArgument(), arguments[4]->getArgument(), arguments[5]->getArgument(), arguments[6]->getArgument(), arguments[7]->getArgument(), arguments[8]->getArgument());
        break;
    case 10:
        method.invoke(processor, Qt::AutoConnection, returnArgument, arguments[0]->getArgument(), arguments[1]->getArgument(), arguments[2]->getArgument(), arguments[3]->getArgument(), arguments[4]->getArgument(), arguments[5]->getArgument(), arguments[6]->getArgument(), arguments[7]->getArgument(), arguments[8]->getArgument(), arguments[9]->getArgument());
        break;
    default:
        qDebug() << "To many arguments, 10 is maximum. This shouö";
    }

    //TODO detect errors
    //TODO async
    QJsonValue resultValue = argument->getJson();
    delete argument;
    emit onSuccess(resultValue);
    //emit onSuccess(QSharedPointer<Response>(new Response(callId, resultValue)));
}

void jsonrpc::Call::setMethod(const QString &methodName)
{
    int methodOffset = processor->metaObject()->methodOffset();
    //From the position after all methods of superclasses(methodOffset) to end of all methods(methodCount).
    for(int methodIndex = methodOffset; methodIndex < processor->metaObject()->methodCount() ; methodIndex++){
        QMetaMethod testMethod = processor->metaObject()->method(methodIndex);
        if(testMethod.name() == methodName){
            method = testMethod;
            return;
        }
    }

    QString errorMessage = QString("Method not found: %1");
    errorMessage = errorMessage.arg(methodName);
    throw Error(Error::Code::MethodNotFound, errorMessage);
}

void jsonrpc::Call::setArguments(const QList<QJsonValue> &providedArguments)
{
    if(providedArguments.size() > 10){
        QString errorMessage = QString("Maximum number of parameters exceeded (%1/10).");
        errorMessage = errorMessage.arg(QString(providedArguments.size()));
        throw Error(Error::Code::InvalidParams, errorMessage);
    }
    QList<int> requiredParameterTypeIds = getRequiredParameterTypes();
    //TODO default/void parameters
    if(providedArguments.size() != requiredParameterTypeIds.size()){
        QString errorMessage = "Parameter amount mismatch (%1:%2)";
        errorMessage = errorMessage.arg(QString(providedArguments.size()), QString(requiredParameterTypeIds.size()));
        throw Error(Error::Code::InvalidParams, errorMessage);
    }

    for(int i = 0;i<requiredParameterTypeIds.size();i++){
        addArgument(requiredParameterTypeIds[i], providedArguments[i]);
    }
}

QList<int> jsonrpc::Call::getRequiredParameterTypes() const
{
    // Get parameter types
    QList<int> requiredParameterTypes;
    for(int i = 0;i<method.parameterCount();i++){
        requiredParameterTypes.push_back(method.parameterType(i));
    }
    return requiredParameterTypes;
}

void jsonrpc::Call::addArgument(const int requiredTypeId, const QJsonValue &providedParameter)
{
    Argument* argument;

    argument = Argument::create(requiredTypeId, providedParameter);

    arguments.push_back(QSharedPointer<Argument>(argument));
}
