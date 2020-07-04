#include "requestmanager.h"

jsonrpc::RequestManager::RequestManager(QObject *target, QObject *parent):
    QObject(parent), processor(target)
{

}

void jsonrpc::RequestManager::processError(const QSharedPointer<jsonrpc::Error> &error)
{

}

void jsonrpc::RequestManager::processResponse(const QSharedPointer<jsonrpc::Response> &response)
{

}
