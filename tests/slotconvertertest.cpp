#ifndef SLOT_CONVERTER_TEST_CPP
#define SLOT_CONVERTER_TEST_CPP

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <slotconverter.h>

#include <QCoreApplication>
#include <QEventLoop>
#include <QJsonValue>
#include <QObject>

#include "mocktarget.hpp"

using namespace testing;
using namespace jsonrpc;

class SlotConverterTests: public ::testing::Test {
 protected:
  void SetUp() override {}

  QList<QString> receivedMessages;
  QList<QSharedPointer<jsonrpc::Request>> receivedRequests;
  QList<QSharedPointer<jsonrpc::Error>> receivedErrors;

  void TearDown() override {}
};

TEST_F(SlotConverterTests, callUncheckedSlotWorks) {
  SlotConverter converter;

  QSharedPointer<jsonrpc::Message> returnValue(nullptr);
  QObject::connect(&converter, &SlotConverter::convertedSlot, [&returnValue](const QSharedPointer<jsonrpc::Message>& message) {
    returnValue = message;
  });

  converter.callRemoteSlotUnchecked("testSlot", {});

  QTime limit = QTime::currentTime().addMSecs(500);
  while(QTime::currentTime() < limit && returnValue == nullptr) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
  }

  ASSERT_EQ(((jsonrpc::Request*)returnValue.get())->getMethodName(), "testSlot");
}

#endif
