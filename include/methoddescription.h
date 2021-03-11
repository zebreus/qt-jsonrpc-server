#ifndef METHODDESCRIPTION_H
#define METHODDESCRIPTION_H

#include <QMetaMethod>
#include <QString>

namespace jsonrpc {

class MethodDescription {
 public:
  MethodDescription();
  MethodDescription(const QMetaMethod& method);
  MethodDescription(const QString& name, const QList<int>& parameterTypeIds, int returnTypeId = QMetaType::Void);
  MethodDescription(const QString& name, const QList<QString>& parameterType, QString returnType = QMetaType::typeName(QMetaType::Void));

  QString getName() const;
  QString getReturnType() const;
  QList<QString> getParameterTypes() const;
  int getReturnTypeId() const;
  QList<int> getParameterTypeIds() const;

  bool operator==(const MethodDescription& that) const;
  bool operator!=(const MethodDescription& that) const;

 private:
  QString name;
  QString returnType;
  QList<QString> parameterTypes;
};

}  // namespace jsonrpc

Q_DECLARE_METATYPE(jsonrpc::MethodDescription)

#endif  // METHODDESCRIPTION_H
