#include "slotconverter.h"

namespace jsonrpc {

jsonrpc::SlotConverter::SlotConverter(QObject* parent): QObject(parent) {}

jsonrpc::SlotConverter::SlotConverter(const QList<jsonrpc::MethodDescription>& slotList, QObject* parent): QObject(parent) {
  registerSlots(slotList);
}

void jsonrpc::SlotConverter::registerSlot(const jsonrpc::MethodDescription& slot) {
  registeredSlots[slot.getName()] = slot;
}

void jsonrpc::SlotConverter::registerSlots(const QList<jsonrpc::MethodDescription>& slotList) {
  for(const jsonrpc::MethodDescription& slot : slotList) {
    registerSlot(slot);
  }
}

bool SlotConverter::checkInterface(const InterfaceDescription& interface) {
  for(const jsonrpc::MethodDescription& slot : registeredSlots) {
    if(!interface.getSlots().contains(slot)) {
      return false;
    }
  }
  return true;
}

bool SlotConverter::callRemoteSlot(const MethodDescription& slot, void** arguments) {
  QList<QSharedPointer<Argument>> argumentList;
  for(int parameterIndex = 0; parameterIndex < slot.getParameterTypeIds().size(); parameterIndex++) {
    const int& parameterType = slot.getParameterTypeIds().at(parameterIndex);
    argumentList.append(QSharedPointer<Argument>(Argument::create(parameterType, arguments[parameterIndex + 1])));
  }

  callRemoteSlotUnchecked(slot.getName(), argumentList);
  return true;
}

bool SlotConverter::callRemoteSlot(const QString& name, void** arguments) {
  QMap<QString, MethodDescription>::iterator slot = registeredSlots.find(name);
  if(slot == registeredSlots.end()) {
    return false;
  }

  return callRemoteSlot(slot.value(), arguments);
}

bool SlotConverter::callRemoteSlot(const MethodDescription& slot, QList<QSharedPointer<Argument>> arguments) {
  // TODO better check
  if(slot.getParameterTypeIds().size() != arguments.size()) {
    return false;
  }
  callRemoteSlotUnchecked(slot.getName(), arguments);
  return true;
}

bool SlotConverter::callRemoteSlot(const QString& name, QList<QSharedPointer<Argument>> arguments) {
  QMap<QString, MethodDescription>::iterator slot = registeredSlots.find(name);
  if(slot == registeredSlots.end()) {
    return false;
  }

  return callRemoteSlot(slot.value(), arguments);
}

void SlotConverter::callRemoteSlotUnchecked(const QString& name, QList<QSharedPointer<Argument>> arguments) {
  QList<QJsonValue> jsonArguments;
  for(const QSharedPointer<Argument>& argument : arguments) {
    jsonArguments.append(argument->getJson());
  }

  QSharedPointer<Request> request(new jsonrpc::Request(name, jsonArguments));
  emit convertedSlot(request);
}

}  // namespace jsonrpc
