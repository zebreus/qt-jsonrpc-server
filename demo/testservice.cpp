#include "testservice.h"

TestService::TestService(QObject* parent): QObject(parent) {
  qDebug() << "Default constructor";
}

TestService::TestService(int number, QString string, QObject* parent): QObject(parent) {
  qDebug() << "Constructed with " << number << " and " << string;
}

QString TestService::echoTest(QString value) {
  qDebug() << "echoTest called with " << value;
  return value;
}

int TestService::additionTest(int a, int b) {
  qDebug() << "additionTest called with " << a << "+" << b << " -> " << a + b;
  return a + b;
}

int TestService::subtractionTest(int a, int b) {
  qDebug() << "subtractionTest called with " << a << "-" << b << " -> " << a - b;
  return a - b;
}
