#ifndef CALLMANAGER_H
#define CALLMANAGER_H

#include <QObject>
#include <QSharedPointer>
#include "request.h"
#include "response.h"
#include "error.h"
#include "call.h"

namespace jsonrpc{

class CallManager : public QObject{
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
}
#endif // CALLMANAGER_H
