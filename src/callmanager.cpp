#include "callmanager.h"

jsonrpc::CallManager::CallManager(QObject *target, QObject *parent):
    QObject(parent), processor(target)
{

}

void jsonrpc::CallManager::processRequest(const QSharedPointer<jsonrpc::Request> &request)
{
    QJsonValue id = request->getId();

    Call* call;
    try{
        call = new Call(processor, request->getMethodName(), request->getArguments(),this);
    }catch(const Error& error){
        QSharedPointer<jsonrpc::Error> response(new jsonrpc::Error(id, error.getCode(), error.getMessage()));
        emit respond(response);
    }

    connect(call, &Call::onError, this, [this, id, call](const Error& error){
        QSharedPointer<jsonrpc::Error> response(new jsonrpc::Error(id, error.getCode(), error.getMessage()));
        call->deleteLater();
        emit respond(response);
    });
    connect(call, &Call::onSuccess, this, [this, id, call](const QJsonValue& result){
        QSharedPointer<jsonrpc::Response> response(new jsonrpc::Response(id, result));
        call->deleteLater();
        emit respond(response);
    });

    call->invoke();
}

void jsonrpc::CallManager::receiveError(QSharedPointer<jsonrpc::Error> error)
{
    emit respond(error);
}

void jsonrpc::CallManager::receiveSuccess(QSharedPointer<jsonrpc::Response> result)
{
    emit respond(result);
}
