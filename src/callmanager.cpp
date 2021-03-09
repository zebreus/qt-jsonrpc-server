#include "callmanager.h"

jsonrpc::CallManager::CallManager(QObject* target, QObject* parent): QObject(parent), processor(target) {}

void jsonrpc::CallManager::processRequest(const QSharedPointer<jsonrpc::Request>& request) {
  QJsonValue id = request->getId();
  bool sendResponse = request->hasId();

  // This should be safe, as long as Call throws or emits onError or onSuccess.
  Call* call;

  try {
    call = new Call(processor, request->getMethodName(), request->getArguments(), this);
  } catch(const exceptions::JsonrpcException& exception) {
    if(sendResponse) {
      emit respond(QSharedPointer<Error>::create(exception.generateError(id)));
    }
    return;
  }

  connect(call, &Call::onError, this, [this, id, call, sendResponse](const Error& error) {
    if(sendResponse) {
      QSharedPointer<Error> response(new jsonrpc::Error(id, error.getCode(), error.getMessage()));
      emit respond(response);
    }
    call->deleteLater();
  });
  connect(call, &Call::onSuccess, this, [this, id, call, sendResponse](const QJsonValue& result) {
    if(sendResponse) {
      QSharedPointer<Response> response(new jsonrpc::Response(id, result));
      emit respond(response);
    }
    call->deleteLater();
  });

  try {
    call->invoke();
  } catch(const exceptions::JsonrpcException& exception) {
    if(sendResponse) {
      emit respond(QSharedPointer<Error>::create(exception.generateError(id)));
    }
    delete call;
  }
}

void jsonrpc::CallManager::receiveError(QSharedPointer<jsonrpc::Error> error) {
  emit respond(error);
}

void jsonrpc::CallManager::receiveSuccess(QSharedPointer<jsonrpc::Response> result) {
  emit respond(result);
}
