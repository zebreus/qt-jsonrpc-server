#include "signalconverter.h"

using namespace jsonrpc;

PrivateSignalConverter::PrivateSignalConverter(QObject* parent): QObject(parent) {}

SignalConverter::SignalConverter(QObject* parent): PrivateSignalConverter(parent), dynamicSlotOffset(100) {}

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
    emit convertedSignal(associatedStrings[id]);
  } else {
    qDebug() << "Something else " << QString::number(call);
  }

  return -1;
}

void SignalConverter::connectIfPossible(QObject* target, const QMetaMethod& method) {
  if(method.access() == QMetaMethod::Public && method.methodType() == QMetaMethod::Signal) {
    int myMethodId = method.methodIndex() + dynamicSlotOffset;
    associatedStrings[myMethodId] = method.name();
    QMetaObject::connect(target, method.methodIndex(), this, myMethodId + metaObject()->methodOffset());
  }
}
