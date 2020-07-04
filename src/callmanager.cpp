#include "callmanager.h"

jsonrpc::CallManager::CallManager(QObject *target, QObject *parent):
    QObject(parent), processor(target)
{

}

void jsonrpc::CallManager::processRequest(const QSharedPointer<jsonrpc::Request> &request)
{
    try{
        //TODO This will not work asynchronously
        //TODO Highly unsafe
        Call* call = new Call(request,processor,this);
        connect(call, &Call::onError, this, &CallManager::receiveError);
        connect(call, &Call::onSuccess, this, &CallManager::receiveSuccess);
        call->invoke();
        delete call;
    }catch(const Error& error){
        QSharedPointer<Message> errorMessage = QSharedPointer<Message>(new Error(error));
        emit respond(errorMessage);
    }
}

void jsonrpc::CallManager::receiveError(QSharedPointer<jsonrpc::Error> error)
{
    emit respond(error);
}

void jsonrpc::CallManager::receiveSuccess(QSharedPointer<jsonrpc::Response> result)
{
    emit respond(result);
}
