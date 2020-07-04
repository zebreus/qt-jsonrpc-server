#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include <QObject>
#include <QSharedPointer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "request.h"
#include "response.h"
#include "error.h"
#include "message.h"
#include "callmanager.h"
#include "requestmanager.h"

namespace jsonrpc{

class MessageProcessor : public QObject{
Q_OBJECT
 public:
    explicit MessageProcessor(QObject* processor, QObject* parent);

  private:
    void processIncomingDocument(const QJsonDocument& message);
    void processIncomingObject(const QJsonObject& message);
    void processIncomingRequest(const QSharedPointer<Request>& request);
    void processIncomingResponse(const QSharedPointer<Response>& response);
    void processIncomingError(const QSharedPointer<Error>& error);

public slots:
    void processIncomingMessage(const QString& message);
    void processOutgoingMessage(const QSharedPointer<Message>& message);

 signals:
    void outgoingMessage(const QString& message);
    void receivedResponse(const QSharedPointer<Response>& response);
    void receivedError(const QSharedPointer<Error>& error);
    void receivedRequest(const QSharedPointer<Request>& request);


 public:
    CallManager* callManager;
    RequestManager* requestManager;
    QObject* processor;
};

}

#endif // MESSAGEPROCESSOR_H
