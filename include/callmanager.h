#ifndef CALLMANAGER_H
#define CALLMANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "call.h"
#include "error.h"
#include "exceptions.h"
#include "request.h"
#include "response.h"

namespace jsonrpc {

class CallManager: public QObject {
  Q_OBJECT

 public:
  CallManager(QObject* target, QObject* parent = nullptr);
  void processRequest(const QSharedPointer<Request>& request);

 public slots:

  void receiveError(QSharedPointer<Error> error);
  void receiveSuccess(QSharedPointer<Response> result);

 signals:
  void respond(QSharedPointer<Message> message);

 public:
  QObject* processor;
};
}  // namespace jsonrpc
#endif  // CALLMANAGER_H
