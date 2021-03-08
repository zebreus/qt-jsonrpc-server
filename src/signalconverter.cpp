#include "signalconverter.h"

using namespace jsonrpc;

PrivateSignalConverter::PrivateSignalConverter(QObject* parent): QObject(parent) {}

SignalConverter::SignalConverter(QObject* parent): PrivateSignalConverter(parent), dynamicSlotOffset(100), messageId(initialMessageId) {}

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
    emit convertedSignal(createMessage(arguments, methodIds[id]));
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
    try {
      QScopedPointer<Argument> argument(Argument::create(parameterType, arguments[parameterIndex + 1]));
      jsonArguments.append(argument->getJson());
    } catch(const exceptions::JsonrpcException& exception) {
      // This can probably happen, if the signal has unsupported types
      // TODO Decide on how to handle it
      // This is not optimal, as Errors are being sent, without a prior request with the same messageId
      QSharedPointer<Error> response(new jsonrpc::Error(exception.generateError(messageId++)));
      return response;
    }
  }

  QSharedPointer<Request> response(new jsonrpc::Request(messageId++, name, jsonArguments));
  return response;
}
