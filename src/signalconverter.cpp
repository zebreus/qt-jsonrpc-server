#include "signalconverter.h"

using namespace jsonrpc;

PrivateSignalConverter::PrivateSignalConverter(QObject* parent): QObject(parent) {}

SignalConverter::SignalConverter(QObject* target, QObject* parent): PrivateSignalConverter(parent), dynamicSlotOffset(100) {
  attach(target);
}

void SignalConverter::attach(QObject* target) {
  const QMetaObject* targetMetaObject = target->metaObject();
  int firstMethod = targetMetaObject->methodOffset();
  int lastMethod = firstMethod + targetMetaObject->methodCount();
  while(firstMethod != lastMethod) {
    const QMetaMethod& method = targetMetaObject->method(firstMethod);
    connectIfPossible(target, method);
    firstMethod++;
  }
}

int SignalConverter::qt_metacall(QMetaObject::Call call, int id, void** arguments) {
  id = QObject::qt_metacall(call, id, arguments);
  if(id < 0) {
    return id;
  }
  if(call == QMetaObject::InvokeMetaMethod) {
    try {
      emit convertedSignal(createMessage(arguments, methodIds[id]));
    } catch(const exceptions::JsonrpcException& exception) {
      // This can probably happen, if the signal has unsupported types
      // TODO Decide on how to handle it
      // This is not optimal, as Errors are being sent, without a prior request with the same messageId
      qDebug() << "Failed to convert signal to message. " << exception.generateError("").getMessage();
      return -1;
    }
  } else {
    qDebug() << "Something else " << QString::number(call);
  }

  return -1;
}

void SignalConverter::connectIfPossible(QObject* target, const QMetaMethod& method) {
  if(method.access() == QMetaMethod::Public && method.methodType() == QMetaMethod::Signal) {
    int myMethodId = method.methodIndex() + dynamicSlotOffset;
    methodIds[myMethodId] = method;
    QMetaObject::connect(target, method.methodIndex(), this, myMethodId + metaObject()->methodOffset());
  }
}

QSharedPointer<Message> SignalConverter::createMessage(void** arguments, const QMetaMethod& method) {
  QString name = method.name();
  QList<QJsonValue> jsonArguments;
  for(int parameterIndex = 0; parameterIndex < method.parameterCount(); parameterIndex++) {
    const int& parameterType = method.parameterType(parameterIndex);
    QScopedPointer<Argument> argument(Argument::create(parameterType, arguments[parameterIndex + 1]));
    jsonArguments.append(argument->getJson());
  }

  QSharedPointer<Request> response(new jsonrpc::Request(name, jsonArguments));
  return response;
}
