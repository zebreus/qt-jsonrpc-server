#include "interfacedescription.h"

namespace jsonrpc {

InterfaceDescription::InterfaceDescription(): version(0.0){};

InterfaceDescription::InterfaceDescription(const QMetaObject& object, double version): version(version) {
  name = object.className();

  int methodOffset = object.methodOffset();
  // From the position after all methods of superclasses(methodOffset) to end of all methods(methodCount).
  for(int methodIndex = methodOffset; methodIndex < object.methodCount(); methodIndex++) {
    QMetaMethod testMethod = object.method(methodIndex);
    if(testMethod.access() != QMetaMethod::Public) {
      continue;
    }
    if(testMethod.methodType() == QMetaMethod::Signal) {
      availableSignals.append(testMethod);
      continue;
    }
    if(testMethod.methodType() == QMetaMethod::Slot) {
      availableSlots.append(testMethod);
      continue;
    }
  }
}

InterfaceDescription::InterfaceDescription(QString name,
                                           QList<MethodDescription> availableSlots,
                                           QList<MethodDescription> availableSignals,
                                           double version)
    : name(name), version(version), availableSlots(availableSlots), availableSignals(availableSignals) {}

QString InterfaceDescription::getName() const {
  return name;
}

double InterfaceDescription::getVersion() const {
  return version;
}

QList<MethodDescription> InterfaceDescription::getSlots() const {
  return availableSlots;
}

QList<MethodDescription> InterfaceDescription::getSignals() const {
  return availableSignals;
}

bool InterfaceDescription::operator==(const InterfaceDescription& that) const {
  return !operator!=(that);
}

bool InterfaceDescription::operator!=(const InterfaceDescription& that) const {
  if(name != that.name) {
    return true;
  }
  if(version != that.version) {
    return true;
  }
  if(availableSlots != that.availableSlots) {
    return true;
  }
  if(availableSignals != that.availableSignals) {
    return true;
  }
  return false;
}

}  // namespace jsonrpc
