#include "methoddescription.h"

namespace jsonrpc {

MethodDescription::MethodDescription() = default;

MethodDescription::MethodDescription(const QMetaMethod& method) {
  name = method.name();
  for(const QByteArray& typeName : method.parameterTypes()) {
    parameterTypes.append(typeName);
  }
  returnType = method.typeName();
}

MethodDescription::MethodDescription(const QString& name, const QList<int>& parameterTypeIds, int returnTypeId): name(name) {
  for(int typeId : parameterTypeIds) {
    parameterTypes.append(QMetaType::typeName(typeId));
  }
  returnType = QMetaType::typeName(returnTypeId);
}

MethodDescription::MethodDescription(const QString& name, const QList<QString>& parameterTypes, QString returnType)
    : name(name), returnType(returnType), parameterTypes(parameterTypes) {}

MethodDescription::MethodDescription(const MethodDescription& that) = default;

QString MethodDescription::getName() const {
  return name;
}

QString MethodDescription::getReturnType() const {
  return returnType;
}

QList<QString> MethodDescription::getParameterTypes() const {
  return parameterTypes;
}

int MethodDescription::getReturnTypeId() const {
  return QMetaType::type(returnType.toUtf8());
}

QList<int> MethodDescription::getParameterTypeIds() const {
  QList<int> typeIds;
  for(const QString& typeName : qAsConst(parameterTypes)) {
    typeIds.append(QMetaType::type(typeName.toUtf8()));
  }
  return typeIds;
}

bool MethodDescription::operator==(const MethodDescription& that) const {
  return !this->operator!=(that);
}

bool MethodDescription::operator!=(const MethodDescription& that) const {
  if(this->name != that.name) {
    return true;
  }
  if(this->returnType != that.returnType) {
    return true;
  }
  if(this->parameterTypes != that.parameterTypes) {
    return true;
  }
  return false;
}

}  // namespace jsonrpc
