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
#include "messageprocessor.h"
#include "signalconverter.h"

namespace jsonrpc {

// TODO Memory management for websocket and target
class Connection: public QObject {
  Q_OBJECT
 public:
  explicit Connection(QWebSocket* websocket, QObject* target, QObject* parent = nullptr);
  ~Connection();
 public slots:
  void disconnect();
 private slots:
  void processRequest(const QSharedPointer<Request>& request);

 private:
  void activateSignals();
  void deactivateSignals();
  void describeInterface();

 signals:
  void incomingMessage(QString message);
  void outgoingMessage(const QString& message);

 private:
  QObject* processor;
  CallManager* callManager;
  QWebSocket* webSocket;
  MessageProcessor* messageProcessor;
  SignalConverter* signalConverter;
};

}  // namespace jsonrpc
#endif  // CONNECTION_H
