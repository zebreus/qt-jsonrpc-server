#ifndef SIGNALRECEIVER_H
#define SIGNALRECEIVER_H

#include <interfacedescription.h>
#include <methoddescription.h>
#include <request.h>

#include <QList>
#include <QMap>
#include <QObject>

namespace jsonrpc {

class SignalReceiver: public QObject {
  Q_OBJECT
 public:
  explicit SignalReceiver(QObject* target, QObject* parent = nullptr);

  void registerSlot(const MethodDescription& slot);
  void registerSlots(const QList<MethodDescription>& slotList);

  // Returns true, if all registered slots are available in interface
  bool checkInterface(const InterfaceDescription& interface);

 public slots:
  void receiveRequest(const QSharedPointer<Request>& message);

 private:
  QObject* target;
};

}  // namespace jsonrpc

/*
int testSignal(const QString& alpha, const QString& beta);
int testSignal2(QString alpha, QString beta);
void testSignal3(QString alpha, QString beta);

// SIGNAL 0
int jsonrpc::SignalReceiver::testSignal(const QString & _t1, const QString & _t2)
{
    int _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t0))), const_cast<void*>(reinterpret_cast<const
void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) }; QMetaObject::activate(this,
&staticMetaObject, 0, _a); return _t0;
}

// SIGNAL 1
int jsonrpc::SignalReceiver::testSignal2(QString _t1, QString _t2)
{
    int _t0{};
    void *_a[] = { const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t0))), const_cast<void*>(reinterpret_cast<const
void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) }; QMetaObject::activate(this,
&staticMetaObject, 1, _a); return _t0;
}

// SIGNAL 2
void jsonrpc::SignalReceiver::testSignal3(QString _t1, QString _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const
void*>(std::addressof(_t2))) }; QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
*/

#endif  // SIGNALRECEIVER_H
