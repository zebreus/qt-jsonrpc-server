#include "call.h"

jsonrpc::Call::Call(const QSharedPointer<Request> &request, QObject *target, QObject *parent):
    QObject(parent), processor(target)
{

    //Throwing an Error object if an error is encountered during construction
    //TODO improve error handling
    callId = request->getId();
    setMethod(request->getMethodName());
    setArguments(request->getArguments());
}

void jsonrpc::Call::invoke()
{
    QMetaType::Type returnType = (QMetaType::Type)method.returnType();
    QVariant returnVariant((QVariant::Type)returnType);
    QGenericReturnArgument returnArgument = QGenericReturnArgument(QMetaType::typeName(returnType), returnVariant.data());

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
    QJsonValue resultValue = QJsonValue::fromVariant(returnVariant);
    emit onSuccess(QSharedPointer<Response>(new Response(callId, resultValue)));
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
    throw Error(callId, Error::Code::MethodNotFound, errorMessage);
}

void jsonrpc::Call::setArguments(const QList<QJsonValue> &providedArguments)
{
    if(providedArguments.size() > 10){
        QString errorMessage = QString("Maximum number of parameters exceeded (%1/10).");
        errorMessage = errorMessage.arg(QString(providedArguments.size()));
        throw Error(callId, Error::Code::InvalidParams, errorMessage);
    }
    QList<QMetaType::Type> requiredParameterTypes = getRequiredParameterTypes();
    if(providedArguments.size() != requiredParameterTypes.size()){
        QString errorMessage = "Parameter amount mismatch (%1:%2)";
        errorMessage = errorMessage.arg(QString(providedArguments.size()), QString(requiredParameterTypes.size()));
        throw Error(callId, Error::Code::InvalidParams, errorMessage);
    }

    for(int i = 0;i<requiredParameterTypes.size();i++){
        addArgument(requiredParameterTypes[i], providedArguments[i]);
    }
}

QList<QMetaType::Type> jsonrpc::Call::getRequiredParameterTypes() const
{
    // Get parameter types
    QList<QMetaType::Type> requiredParameterTypes;
    for(int i = 0;i<method.parameterCount();i++){
        requiredParameterTypes.push_back((QMetaType::Type)method.parameterType(i));
    }
    return requiredParameterTypes;
}

void jsonrpc::Call::addArgument(const QMetaType::Type &requiredType, const QJsonValue &providedParameter)
{
    Argument* argument;

    //TODO move this to a factory method in argument
    switch(requiredType){
    case QMetaType::Bool :
        argument = new BoolArgument(providedParameter);
        break;
    case QMetaType::Int :
        argument = new IntArgument(providedParameter);
        break;
    case QMetaType::UInt :
        argument = new UIntArgument(providedParameter);
        break;
    case QMetaType::QChar :
        argument = new QCharArgument(providedParameter);
        break;
    case QMetaType::QString :
        argument = new QStringArgument(providedParameter);
        break;
    case QMetaType::QJsonValue :
        argument = new QJsonValueArgument(providedParameter);
        break;
    case QMetaType::Double :
        argument = new DoubleArgument(providedParameter);
        break;
    default:
        qDebug()  << "Request has unsupported type "<< "Not all types are implemented yet.";
        QString errorMessage = "Invalid type of parameter %1, %2 required.";
        errorMessage = errorMessage.arg(QString(arguments.size()+1), QString(QMetaType::typeName(requiredType)));
        throw Error(callId, Error::Code::InvalidParams, errorMessage);
    }
    arguments.push_back(QSharedPointer<Argument>(argument));
}
