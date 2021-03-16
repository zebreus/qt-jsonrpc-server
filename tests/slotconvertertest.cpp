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
  void SetUp() override {
    QObject::connect(&converter, &SlotConverter::convertedSlot, [this](const QSharedPointer<jsonrpc::Message>& message) {
      returnValue = message;
    });
  }

  inline bool processEvents(int millis, auto stopCondition) {
    QTime limit = QTime::currentTime().addMSecs(millis);
    while(QTime::currentTime() < limit && stopCondition()) {
      QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }
    return !stopCondition();
  }

  inline bool processEvents(int millis = 500) {
    return processEvents(millis, [this]() {
      return returnValue == nullptr;
    });
  }

  SlotConverter converter;
  QSharedPointer<jsonrpc::Message> returnValue;

  void TearDown() override {}
};

TEST_F(SlotConverterTests, callUncheckedSlotWithoutParamtersWorks) {
  converter.callRemoteSlotUnchecked("testSlot", {});

  processEvents();

  ASSERT_NE(returnValue, nullptr);
  EXPECT_EQ(((jsonrpc::Request*)returnValue.get())->getMethodName(), "testSlot");
  EXPECT_EQ(((jsonrpc::Request*)returnValue.get())->getArguments().size(), 0);
}

TEST_F(SlotConverterTests, callUncheckedSlotWithParametersWorks) {
  QString stringArgumentValue = "aaa";
  QSharedPointer<Argument> stringArgument(Argument::create(QMetaType::QString, (void*)&stringArgumentValue));
  converter.callRemoteSlotUnchecked("testSlot", {stringArgument});

  processEvents();

  ASSERT_NE(returnValue, nullptr);
  EXPECT_EQ(((jsonrpc::Request*)returnValue.get())->getMethodName(), "testSlot");
  EXPECT_EQ(((jsonrpc::Request*)returnValue.get())->getArguments().size(), 1);
  EXPECT_EQ(((jsonrpc::Request*)returnValue.get())->getArguments().at(0), stringArgumentValue);
}

#endif
