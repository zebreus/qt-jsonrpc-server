#ifndef SIGNALCONVERTER_H
#define SIGNALCONVERTER_H

#include <argument.h>
#include <exceptions.h>
#include <message.h>
#include <request.h>

#include <QDebug>
#include <QMap>
#include <QMetaMethod>
#include <QMetaObject>
#include <QObject>
#include <QString>

namespace jsonrpc {

// Base class for SignalConverter. This is necessary, because the Q_OBJECT macro cannot be used in SignalConverter, because it implements
// the qt_metacall function itself
class PrivateSignalConverter: public QObject {
  Q_OBJECT
 public:
  explicit PrivateSignalConverter(QObject* parent = nullptr);
 signals:
  void convertedSignal(const QSharedPointer<Message>& message);
};

// Attaches to a QObject and converts all public signals it emits to Request messages
class SignalConverter: public PrivateSignalConverter {
 public:
  SignalConverter(QObject* target, QObject* parent = nullptr);
  int qt_metacall(QMetaObject::Call, int, void**) override;

 private:
  void attach(QObject* target);
  void connectIfPossible(QObject* target, const QMetaMethod& method);
  QSharedPointer<Message> createMessage(void** arguments, const QMetaMethod& method);

  int dynamicSlotOffset;
  QMap<int, QMetaMethod> methodIds;
};

}  // namespace jsonrpc

#endif  // SIGNALCONVERTER_H
