#ifndef SIGNAL_CONVERTER_TEST_CPP
#define SIGNAL_CONVERTER_TEST_CPP

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <signalconverter.h>

#include <QCoreApplication>
#include <QEventLoop>
#include <QJsonValue>
#include <QObject>

#include "calltest.hpp"

using namespace testing;
using namespace jsonrpc;

TEST(signalConverterTests, testSignalsGetConverted) {
  MockTarget emitter;
  SignalConverter converter;
  converter.attach(&emitter);

  QSharedPointer<jsonrpc::Message> returnValue(nullptr);
  QObject::connect(&converter, &SignalConverter::convertedSignal, [&returnValue](const QSharedPointer<jsonrpc::Message>& message) {
    returnValue = message;
  });

  emit emitter.signalA();

  QTime limit = QTime::currentTime().addMSecs(500);
  while(QTime::currentTime() < limit && returnValue == nullptr) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
  }

  ASSERT_EQ(((jsonrpc::Request*)returnValue.get())->getMethodName(), "signalA");
}

TEST(signalConverterTests, testMethodsAndSlotsDoNotGetConverted) {
  MockTarget emitter;
  SignalConverter converter;
  converter.attach(&emitter);

  bool convertedSignal = false;
  QObject::connect(&converter, &SignalConverter::convertedSignal, [&convertedSignal](const QSharedPointer<jsonrpc::Message>& message) {
    convertedSignal = true;
  });

  // emit is used for clarity
  // Slot
  emit emitter.noParams();
  // Method
  emit emitter.emptyMethod();

  QTime limit = QTime::currentTime().addMSecs(200);
  while(QTime::currentTime() < limit && convertedSignal == false) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
  }

  ASSERT_FALSE(convertedSignal);
}

TEST(signalConverterTests, convertedSignalIsNotification) {
  MockTarget emitter;
  SignalConverter converter;
  converter.attach(&emitter);

  QSharedPointer<jsonrpc::Message> returnValue(nullptr);
  QObject::connect(&converter, &SignalConverter::convertedSignal, [&returnValue](const QSharedPointer<jsonrpc::Message>& message) {
    returnValue = message;
  });

  emit emitter.signalA();

  QTime limit = QTime::currentTime().addMSecs(500);
  while(QTime::currentTime() < limit && returnValue == nullptr) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
  }

  ASSERT_EQ(((jsonrpc::Request*)returnValue.get())->getMethodName(), "signalA");
  // Notifications do not have ids
  ASSERT_EQ(((jsonrpc::Request*)returnValue.get())->hasId(), false);
}

#endif
