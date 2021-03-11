#ifndef INTERFACEDESCRIPTION_H
#define INTERFACEDESCRIPTION_H

#include <methoddescription.h>

#include <QList>
#include <QString>

namespace jsonrpc {

class InterfaceDescription {
 public:
  InterfaceDescription();
  InterfaceDescription(const QMetaObject& object, double version = 0.0);
  InterfaceDescription(QString name, QList<MethodDescription> availableSlots, QList<MethodDescription> availableSignals, double version = 0.0);

  QString getName() const;
  double getVersion() const;
  QList<MethodDescription> getSlots() const;
  QList<MethodDescription> getSignals() const;

  bool operator==(const InterfaceDescription& that) const;
  bool operator!=(const InterfaceDescription& that) const;

 private:
  QString name;
  double version;
  QList<MethodDescription> availableSlots;
  QList<MethodDescription> availableSignals;
};

}  // namespace jsonrpc

Q_DECLARE_METATYPE(jsonrpc::InterfaceDescription)

#endif  // INTERFACEDESCRIPTION_H
