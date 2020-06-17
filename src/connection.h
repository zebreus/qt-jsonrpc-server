#ifndef JSONRPCCONNECTION_H
#define JSONRPCCONNECTION_H

#include <QObject>
#include <QMetaType>
#include <QMetaMethod>
#include <QReturnArgument>
#include <QtWebSockets/QWebSocket>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <cmath>
#include <QCoreApplication>

namespace jsonrpc{

//Intermediate class that the JsonRpcServer inherits from, as template classes cannot use the QObject macro
//All signals and slots have to be defined here
class IntermediateConnection : public QObject {
    Q_OBJECT
public:
    explicit IntermediateConnection(QObject *parent = nullptr): QObject(parent){}
private Q_SLOTS:
    virtual void receiveMessage(const QString& message) {}
    virtual void disconnected() {}
public: signals:
    void sendResult(const QString& message);
};

//Only allow templates for classes that are based on QObject
template<class T, bool = std::is_base_of<QObject, T>::value>
class Connection : public IntermediateConnection {};

template<class T>
class Connection<T, true> : public IntermediateConnection
{
public:
    explicit Connection(QWebSocket* socket, QObject *parent = nullptr);
    ~Connection();
    void receiveMessage(const QString& message) override;
    void disconnected() override;

// Methods for json processing, maybe put them somewhere else
    bool processJsonRpc(const QJsonObject& message);
    bool processRequest(const QJsonObject& message);
    bool processResult(const QJsonObject& message);


private:
    QWebSocket* webSocket;
    T* processor;
};

template<typename T>
Connection<T, true>::Connection(QWebSocket *socket, QObject *parent) : IntermediateConnection(parent), webSocket(socket)
{
    processor = new T(this);
    connect(webSocket, &QWebSocket::textMessageReceived,
        this, &Connection<T>::receiveMessage);
    connect(webSocket, &QWebSocket::disconnected,
        this, &Connection<T>::disconnected);
}

template<class T>
Connection<T,true>::~Connection(){
    webSocket->close();
}

template<typename T>
void Connection<T, true>::receiveMessage(const QString &message)
{
    qDebug() << "processing message " << message;

    //Try to create JSON object from message
    QJsonParseError err;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(message.toUtf8(), &err);
    if (err.error != QJsonParseError::NoError) {
        qDebug() << "QJsonParseError while processing message: " << message;
        //TODO handle error
        return;
    }

    //Decide if message is a batch and handle
    if(jsonDocument.isObject()){
        //Single call
        QJsonObject jsonObj = jsonDocument.object();
        processJsonRpc(jsonObj);
    }else if(jsonDocument.isArray()){
        //Batch call
        QJsonArray jsonArr = jsonDocument.array();
        for(QJsonValue val : jsonArr){
            if(val.isObject()){
                processJsonRpc(val.toObject());
            }
        }
    }else{
        qDebug() << "JsonDocument is probably empty";
        //TODO handle error
        return;
    }
}

template<class T>
bool Connection<T, true>::processJsonRpc(const QJsonObject &message)
{
    // Check if it has a valid rpc field
    QJsonValue jsonrpcValue = message.value("jsonrpc");
    if((jsonrpcValue.type() != QJsonValue::String) || (jsonrpcValue.toString() != "2.0")){
        qDebug() << "jsonrpc value incorrect.";
        return false;
    }

    // Check if id field is has correct type or is not existant
    QJsonValue idValue = message.value("id");
    if(!( (idValue.type() == QJsonValue::String) || (idValue.type() == QJsonValue::Double) || (idValue.type() == QJsonValue::Null))){
        qDebug() << "id type incorrect.";
        return false;
    }

    //Check if it is a message or a result
    QJsonValue methodValue = message.value("method");
    QJsonValue resultValue = message.value("result");
    if(methodValue.type() != QJsonValue::Undefined && resultValue.type() == QJsonValue::Undefined){
        //Method
        //Check type
        if(methodValue.type() != QJsonValue::String){
            qDebug() << "method type incorrect";
            return false;
        }

        QJsonValue paramsValue = message.value("params");
        if(!( (paramsValue.type() == QJsonValue::Array) || (paramsValue.type() == QJsonValue::Object) || (paramsValue.type() == QJsonValue::Undefined))){
            qDebug() << "params type incorrect. Valid types are array, object or undefined.";
            return false;
        }

        return processRequest(message);
    }else if(resultValue.type() != QJsonValue::Undefined && methodValue.type() == QJsonValue::Undefined){
        //Result
        //TODO implement result handling
        qDebug() << "Result handling not implemented";
        return false;

    }else{
        //Not method or result
        qDebug() << "No method or result defined.";
        return false;
    }
}

template<class T>
inline int deletePointer(void* t){
    ((T*)t)->~T();
    return 0;
}

template<class T>
bool Connection<T, true>::processRequest(const QJsonObject &message)
{
    // Find method
    QString methodName = message.value("method").toString();
    QMetaMethod method;
    int methodOffset = processor->metaObject()->methodOffset();
    //From the position after all methods of superclasses(methodOffset) to end of all methods(methodCount).
    for(int methodIndex = methodOffset; methodIndex < processor->metaObject()->methodCount() ; methodIndex++){
        method = processor->metaObject()->method(methodIndex);
        if(method.name() == methodName){
            break;
        }
    }

    // Fetch parameters into list for easier processing
    QList<QJsonValue> providedParameters;
    if(message.value("params").isArray()){
        for(QJsonValue val : message.value("params").toArray()){
            providedParameters.append(val);
        }
    }else if(message.value("params").isObject()){
        //TODO probably also valid, but not implemented yet
        qDebug() << "Parameter passing by name is not implemented.";
        return false;
    }else{
        //No parameters is ok
    }

    // Check parameter count
    if(method.parameterCount() > 10){
        qDebug() << "Too many parameters";
        return false;
    }
    if(method.parameterCount() != providedParameters.size()){
        qDebug() << "Parameter count mismatch. "
                 << "Provided: " << providedParameters.size() << " Needed: " << method.parameterCount();
        return false;
    }

    // Get parameter types
    QList<QMetaType::Type> requiredParameterTypes;
    for(int i = 0;i<method.parameterCount();i++){
        requiredParameterTypes.push_back((QMetaType::Type)method.parameterType(i)) ;
    }

    // Convert paramaters to GenericArguments of correct datatype
    // Really good explanation of QGenericArgument: https://stackoverflow.com/questions/50486892/creating-qgenericargument
    // I have to allocate memory for every argument value, as the value is not copied by using Q_ARG.
    // I create QVariants that allocate the memory and manage the QVariants in a list of smart pointer, so they get deallocated when the function returns
    // TODO do not abuse QVariant for this
    QList<QGenericArgument> parameters;
    QList<QSharedPointer<QVariant>> variantPointers;
    for(int i = 0; i< providedParameters.size(); i++){
        const QMetaType::Type requiredType = requiredParameterTypes.at(i);
        const QJsonValue& providedParameter = providedParameters.at(i);
        // Argument for the function
        QGenericArgument argument;
        // Value will be filled
        QVariant* value = new QVariant((QVariant::Type)requiredType);
        variantPointers.push_back(QSharedPointer<QVariant>(value));


        switch(requiredType){
        case QMetaType::Bool :
            if( providedParameter.isBool() ){
                value->setValue(providedParameter.toBool());
                argument = Q_ARG(bool, *((bool*)value->data()));
            }else{
                qDebug() << "Parameter type of parameter " << i << " cannot be converted to bool";
                return false;
            }
            break;
        case QMetaType::Int :
            if( providedParameter.isDouble() ){
                double doubleValue = providedParameter.toDouble();
                double integerPart;
                if(std::modf(doubleValue, &integerPart) != 0.0){
                    qDebug() << "Cannot convert double to integer, because it is not integer";
                    return false;
                }
                if (integerPart > (double)INT_MAX || integerPart < (double)INT_MIN){
                    qDebug() << "Cannot convert double to integer, because it is out of range";
                    return false;
                }
                value->setValue((int)integerPart);
                argument = Q_ARG(int, *((int*)value->data()));
            }else{
                qDebug() << "Parameter type of parameter " << i << " cannot be converted to int";
                return false;
            }
             break;
        case QMetaType::UInt :
            if( providedParameter.isDouble() ){
                double doubleValue = providedParameter.toDouble();
                double integerPart;
                if(std::modf(doubleValue, &integerPart) != 0.0){
                    qDebug() << "Cannot convert double to unsigned integer, because it is not integer";
                    return false;
                }
                if (integerPart > (double)UINT_MAX || integerPart < (double)0){
                    qDebug() << "Cannot convert double to unsigned integer, because it is out of range";
                    return false;
                }
                value->setValue((unsigned int)integerPart);
                argument = Q_ARG(unsigned int, *((unsigned int*)value->data()));
            }else{
                qDebug() << "Parameter type of parameter " << i << " cannot be converted to unsigned int";
                return false;
            }
            break;
        case QMetaType::Double :
            if( providedParameter.isDouble() ){
                value->setValue(providedParameter.toDouble());
                argument = Q_ARG(double, *((double*)value->data()));
            }else{
                qDebug() << "Parameter type of parameter " << i << " cannot be converted to double";
                return false;
            }
            break;
        case QMetaType::QChar :
            if( providedParameter.isString() ){
                QString stringValue = providedParameter.toString();
                if(stringValue.length() != 1){
                    qDebug() << "Cannot convert string to char, because it is not one char long";
                    return false;
                }
                value->setValue((QChar)stringValue.front());
                argument = Q_ARG(QChar, *((QChar*)value->data()));
            }else{
                qDebug() << "Parameter type of parameter " << i << " cannot be converted to QChar";
                return false;
            }
            break;
        case QMetaType::QString :
            if( providedParameter.isString() ){
                value->setValue(providedParameter.toString());
                argument = Q_ARG(QString, *((QString*)value->data()));

            }else{
                qDebug() << "Parameter type of parameter " << i << " cannot be converted to QString";
                return false;
            }
            break;
        case QMetaType::QJsonValue :
            //TODO Probably the value should be checked somehow
            if( true ){
                value->setValue(providedParameter);
                argument = Q_ARG(QJsonValue, *((QJsonValue*)value->data()));
            }else{
                qDebug() << "Parameter type of parameter " << i << " cannot be converted to QJsonValue";
                return false;
            }
            break;
        default:
            //TODO Add more types
            qDebug()  << "Request has unsupported type "<< "Not all types are implemented yet.";
            return false;
        }
        parameters.push_back(argument);
    }

    QJsonValue numberVal(6);
    QJsonValue textVal("test");

    QVariant testvariant(QVariant::Int);
    testvariant.toInt();
    testvariant.setValue(textVal.toVariant());

    qDebug() << "testvar:" << testvariant;

    //Create a generic return argument
    QMetaType::Type returnType = (QMetaType::Type)method.returnType();
    QVariant returnVariant((QVariant::Type)returnType);
    QGenericReturnArgument returnArgument = QGenericReturnArgument(QMetaType::typeName(returnType), returnVariant.data());

    qDebug() << "INVOKING!";
    // The Q_ARG makro only creates a wrapper, but does not copy the values
    // I think invoke copies the value if it uses a QueuedConnection
    // Do the call
    switch(parameters.size()){
    case 0:
        method.invoke(processor, Qt::AutoConnection, returnArgument);
        break;
    case 1:
        method.invoke(processor, Qt::AutoConnection, returnArgument, parameters[0]);
        break;
    case 2:
        method.invoke(processor, Qt::AutoConnection, returnArgument, parameters[0], parameters[1]);
        break;
    case 3:
        method.invoke(processor, Qt::AutoConnection, returnArgument, parameters[0], parameters[1], parameters[2]);
        break;
    case 4:
        method.invoke(processor, Qt::AutoConnection, returnArgument, parameters[0], parameters[1], parameters[2], parameters[3]);
        break;
    case 5:
        method.invoke(processor, Qt::AutoConnection, returnArgument, parameters[0], parameters[1], parameters[2], parameters[3], parameters[4]);
        break;
    case 6:
        method.invoke(processor, Qt::AutoConnection, returnArgument, parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5]);
        break;
    case 7:
        method.invoke(processor, Qt::AutoConnection, returnArgument, parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6]);
        break;
    case 8:
        method.invoke(processor, Qt::AutoConnection, returnArgument, parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7]);
        break;
    case 9:
        method.invoke(processor, Qt::AutoConnection, returnArgument, parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8]);
        break;
    case 10:
        method.invoke(processor, Qt::AutoConnection, returnArgument, parameters[0], parameters[1], parameters[2], parameters[3], parameters[4], parameters[5], parameters[6], parameters[7], parameters[8], parameters[9]);
        break;
    default:
        qDebug() << "To many arguments, 10 is maximum. This shouö";
    }

    //Build result message
    //TODO better implementation and probably somewhere else
    QJsonObject resultObject;
    QJsonValue resultValue = QJsonValue::fromVariant(returnVariant);
    resultObject.insert("jsonrpc","2.0");
    resultObject.insert("result", resultValue);
    resultObject.insert("id", message.value("id"));
    QJsonDocument resultDocument;
    resultDocument.setObject(resultObject);
    //emit sendResult(resultDocument.toJson());
    webSocket->sendBinaryMessage(resultDocument.toJson(QJsonDocument::Compact));

    qDebug() << "RESULT: " << returnVariant;

    return true;
}

template<class T>
bool Connection<T, true>::processResult(const QJsonObject &message)
{
    qDebug() << "processResult not implemented yet";
    return true;
}

template<typename T>
void Connection<T, true>::disconnected()
{
    qDebug() << "disconnected";
}

}
#endif // JSONRPCCONNECTION_H
