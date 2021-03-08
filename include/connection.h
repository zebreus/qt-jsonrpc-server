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

#include "messageprocessor.h"

namespace jsonrpc {

// TODO Memory management for websocket and target
class Connection: public QObject {
  Q_OBJECT
 public:
  explicit Connection(QWebSocket* websocket, QObject* target, QObject* parent = nullptr);
  ~Connection();
 public slots:
  void disconnect();

 signals:
  void incomingMessage(QString message);
  void outgoingMessage(const QString& message);

 public:
  QObject* processor;
  CallManager* callManager;
  QWebSocket* webSocket;
  MessageProcessor* messageProcessor;
};

}  // namespace jsonrpc
#endif  // CONNECTION_H
