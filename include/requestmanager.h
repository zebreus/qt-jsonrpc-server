#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include <QSharedPointer>

#include "error.h"
#include "message.h"
#include "request.h"
#include "response.h"

namespace jsonrpc {

class RequestManager: public QObject {
  Q_OBJECT
 public:
  RequestManager(QObject* target, QObject* parent);
  void processError(const QSharedPointer<Error>& error);
  void processResponse(const QSharedPointer<Response>& response);

 signals:
  void sendingRequest(const QSharedPointer<Request>& request);

 private:
  QList<Request*> activeRequests;
  QObject* processor;
};

}  // namespace jsonrpc

#endif  // REQUESTMANAGER_H
