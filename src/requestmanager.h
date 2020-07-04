#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>
#include <QSharedPointer>
#include "request.h"
#include "message.h"
#include "error.h"
#include "response.h"

namespace jsonrpc{

class RequestManager : public QObject{
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

}

#endif // REQUESTMANAGER_H
