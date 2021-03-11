#ifndef CONNECTION_H
#define CONNECTION_H

#include <QCoreApplication>
#include <QMetaMethod>
#include <QMetaType>
#include <QObject>
#include <QReturnArgument>
#include <QString>
#include <QWebSocket>
#include <QtWebSockets/QWebSocket>
#include <cmath>

#include "callmanager.h"
#include "interfacedescription.h"
#include "messageprocessor.h"
#include "signalconverter.h"

namespace jsonrpc {

// TODO Memory management for websocket and target
class Connection: public QObject {
  Q_OBJECT
 public:
  explicit Connection(QObject* target, QObject* parent = nullptr);
  ~Connection();
 public slots:
  void disconnect();
  void receiveMessage(const QString& message);
 signals:
  void sendMessage(const QString& message);

 private slots:
  void processRequest(const QSharedPointer<Request>& request);

 private:
  void activateSignals();
  void deactivateSignals();
  QJsonValue describeInterface();

 private:
  QObject* processor;
  CallManager* callManager;
  MessageProcessor* messageProcessor;
  SignalConverter* signalConverter;
};

}  // namespace jsonrpc
#endif  // CONNECTION_H
