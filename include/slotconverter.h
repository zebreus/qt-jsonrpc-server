#ifndef SLOTCONVERTER_H
#define SLOTCONVERTER_H

#include <argument.h>
#include <methoddescription.h>
#include <request.h>

#include <QObject>

namespace jsonrpc {

// Converts slot calls to Notification messages
// The target object needs to call callRemoteSlot to make a slot call
// There are three ways to call slots
class SlotConverter: public QObject {
  Q_OBJECT
 public:
  explicit SlotConverter(QObject* parent = nullptr);
  explicit SlotConverter(const QList<MethodDescription>& slotList, QObject* parent = nullptr);
  // Add method description
  void registerSlot(const MethodDescription& slot);
  void registerSlots(const QList<MethodDescription>& slotList);

  // Returns true, if all registered slots are available in interface
  bool checkInterface(const InterfaceDescription& interface);

  // TODO Maybe not do this here, as it will probably crash, if arguments does not match method
  // Call a remote slot by method description
  // Returns false if there is no compatible method
  bool callRemoteSlot(const MethodDescription& method, void** arguments);
  // Call a remote slot by registered method description
  // Returns false if there is no compatible method
  bool callRemoteSlot(const QString& name, void** arguments);
  // Call a remote slot with the given arguments and check with method description
  // Returns false if there is no compatible method
  bool callRemoteSlot(const MethodDescription& name, QList<QSharedPointer<Argument>> arguments);
  // Call a remote slot with the given arguments and check with method description
  // Returns false if there is no compatible method
  bool callRemoteSlot(const QString& name, QList<QSharedPointer<Argument>> arguments);

  // Call a remote slot with the given arguments
  void callRemoteSlotUnchecked(const QString& name, QList<QSharedPointer<Argument>> arguments);

 signals:
  void convertedSlot(const QSharedPointer<Message> message);

 private:
  QMap<QString, MethodDescription> registeredSlots;
};

}  // namespace jsonrpc

#endif  // SLOTCONVERTER_H
