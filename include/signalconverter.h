#ifndef SIGNALCONVERTER_H
#define SIGNALCONVERTER_H

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
  void convertedSignal(const QString& message);
};

// Attaches to a QObject and converts all public signals it emits to Request messages
class SignalConverter: public PrivateSignalConverter {
 public:
  SignalConverter(QObject* parent = nullptr);
  void attach(QObject* target);
  int qt_metacall(QMetaObject::Call, int, void**) override;

 private:
  void connectIfPossible(QObject* target, const QMetaMethod& method);

  int dynamicSlotOffset;
  QMap<int, QString> associatedStrings;
};

}  // namespace jsonrpc

#endif  // SIGNALCONVERTER_H
