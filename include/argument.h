#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <exceptions.h>
#include <interfacedescription.h>
#include <methoddescription.h>

#include <QDate>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMetaType>
#include <QSize>
#include <cmath>

#if defined(QT_GUI_LIB) or defined(QT_MODULE_GUI)
#include <QBitArray>
#include <QColor>
#include <QColorSpace>
#include <QCursor>
#include <QIcon>
#include <QLine>
#include <QPalette>
#include <QPolygon>
#include <QTextLength>
#include <QtGui>
#endif
#if defined(QT_WIDGETS_LIB) or defined(QT_MODULE_WIDGETS)
#include <QSizePolicy>
#endif

namespace jsonrpc {

class Argument {
 public:
  QGenericArgument& getArgument();
  virtual QJsonValue getJson() = 0;
  virtual ~Argument();

  template<typename T>
  static Argument* create(const int requiredTypeId, T const& value);
  static Argument* create(const int requiredTypeId);

 protected:
  template<typename T>
  static Argument* createArgument(const QJsonValue& jsonValue);
  template<typename T>
  static Argument* createArgument(void* const& jsonValue);
  static QString getTypeName(const QJsonValue& value);
  static QString getTypeName(void* const& value);
  Argument();

 private:
  Argument(Argument const&) = delete;
  Argument& operator=(Argument const&) = delete;

 protected:
  QGenericArgument argument;
};

template<typename T>
class ArgumentImplementation: public Argument {
 public:
  ~ArgumentImplementation() override;
  ArgumentImplementation(const QJsonValue& argument);
  ArgumentImplementation(T* const& argument);
  ArgumentImplementation();

  QJsonValue getJson() override;

 private:
  void setValue(const T& t);
  T* value;
};

class VoidArgument: public Argument {
 public:
  VoidArgument();
  VoidArgument(const QJsonValue& value);
  QJsonValue getJson() override;

 private:
  int nothing[0];
};

}  // namespace jsonrpc

#endif  // ARGUMENT_H
