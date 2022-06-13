#include "signalreceiver.h"

jsonrpc::SignalReceiver::SignalReceiver(QObject* target, QObject* parent): QObject(parent), target(target) {}

void jsonrpc::SignalReceiver::receiveRequest(const QSharedPointer<jsonrpc::Request>&) {}
