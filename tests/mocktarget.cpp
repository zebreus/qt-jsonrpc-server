#include "mocktarget.hpp"

SuperMock::SuperMock(QObject* parent): QObject(parent) {}

SuperMock::~SuperMock() {}

int SuperMock::superSlot() {
  return 564;
}

MockTarget::MockTarget(QObject* parent): SuperMock(parent), constructorNumber(0) {}

MockTarget::MockTarget(int number, QObject* parent): SuperMock(parent), constructorNumber(number) {}

MockTarget::~MockTarget() {}

int MockTarget::emptyMethod() {
  return 345;
}

int MockTarget::addNumbers(int a, int b) {
  return a + b;
}

QString MockTarget::echoString(const QString& input) {
  return input;
}

int MockTarget::noParams() {
  return 564;
}

void MockTarget::emitSignalA() {
  emit signalA();
}

void MockTarget::emitSignalB() {
  emit signalB("text");
}

int MockTarget::privateSlot() {
  return 564;
}
