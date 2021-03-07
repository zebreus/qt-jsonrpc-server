#ifndef CONNECTION_H
#define CONNECTION_H

#include <QCoreApplication>
#include <QMetaMethod>
#include <QMetaType>
#include <QObject>
#include <QReturnArgument>
#include <QString>
#include <QtWebSockets/QWebSocket>
#include <cmath>

#include "messageprocessor.h"

namespace jsonrpc {

// TODO Memory management for websocket and target
class Connection: public QObject {
  Q_OBJECT
 public:
  explicit Connection(QWebSocket* websocket, QObject* target, QObject* parent = nullptr);
 public slots:
  void receiveIncomingMessage(const QString& message);
  void receiveOutgoingMessage(const QString& message);
  void disconnect();

 signals:
  void incomingMessage(QString message);
  void outgoingMessage(const QString& message);

 public:
  QObject* processor;
  QWebSocket* webSocket;
  MessageProcessor* messageProcessor;
};

}  // namespace jsonrpc
#endif  // CONNECTION_H
