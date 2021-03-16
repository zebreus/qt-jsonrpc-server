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

TEST_F(SlotConverterTests, callSlotWorksWithRegisteredSlot) {
  QString stringArgumentValue = "aaa";
  QSharedPointer<Argument> stringArgument(Argument::create(QMetaType::QString, (void*)&stringArgumentValue));

  MethodDescription slot("testSlot", {QMetaType::QString});
  converter.registerSlot(slot);

  ASSERT_TRUE(converter.callRemoteSlot("testSlot", {stringArgument}));
  processEvents();

  ASSERT_NE(returnValue, nullptr);
  EXPECT_EQ(((jsonrpc::Request*)returnValue.get())->getMethodName(), "testSlot");
}

TEST_F(SlotConverterTests, callSlotWorksWithRegisteredSlotListWorks) {
  QString stringArgumentValue = "aaa";
  QSharedPointer<Argument> stringArgument(Argument::create(QMetaType::QString, (void*)&stringArgumentValue));

  MethodDescription slot("testSlot", {QMetaType::QString});
  converter.registerSlots({slot});

  ASSERT_TRUE(converter.callRemoteSlot("testSlot", {stringArgument}));
  processEvents();

  ASSERT_NE(returnValue, nullptr);
  EXPECT_EQ(((jsonrpc::Request*)returnValue.get())->getMethodName(), "testSlot");
}

TEST_F(SlotConverterTests, callRemoteSlotDoesNotWorkWithUnregisteredSlot) {
  QString stringArgumentValue = "aaa";
  QSharedPointer<Argument> stringArgument(Argument::create(QMetaType::QString, (void*)&stringArgumentValue));

  MethodDescription registeredSlot("registeredSlot", {QMetaType::QString});
  MethodDescription unregisteredSlot("unregisteredSlot", {QMetaType::QString});
  converter.registerSlot(registeredSlot);

  EXPECT_FALSE(converter.callRemoteSlot("unregisteredSlot", {stringArgument}));
  EXPECT_FALSE(converter.callRemoteSlot("unregisteredSlot", {}));
  EXPECT_FALSE(converter.callRemoteSlot("unregisteredSlot", {stringArgument, stringArgument}));
  EXPECT_FALSE(converter.callRemoteSlot("registeredSlot", QList<QSharedPointer<Argument>>()));
  EXPECT_FALSE(converter.callRemoteSlot("registeredSlot", {stringArgument, stringArgument}));

  processEvents(100);

  ASSERT_EQ(returnValue, nullptr);
}

TEST_F(SlotConverterTests, checkInterfaceWorks) {
  QString stringArgumentValue = "aaa";
  QSharedPointer<Argument> stringArgument(Argument::create(QMetaType::QString, (void*)&stringArgumentValue));

  MethodDescription slotA("slotA", {QMetaType::QString});
  MethodDescription slotB("slotB", {QMetaType::QString});
  MethodDescription slotBWithOtherParams("slotB", {QMetaType::Int});
  MethodDescription slotC("slotC", {QMetaType::QString});

  converter.registerSlots({slotA, slotB});
  InterfaceDescription incompatibleInterface1("incompatible1", {slotA}, {});
  InterfaceDescription incompatibleInterface2("incompatible2", {slotB}, {});
  InterfaceDescription incompatibleInterface3("incompatible3", {slotC}, {});
  InterfaceDescription incompatibleInterface4("incompatible4", {slotA, slotC}, {});
  InterfaceDescription incompatibleInterface5("incompatible5", {slotBWithOtherParams, slotA}, {});
  InterfaceDescription compatibleInterface1("compatible1", {slotB, slotA}, {});
  InterfaceDescription compatibleInterface2("compatible2", {slotB, slotC, slotA}, {});

  ASSERT_TRUE(converter.checkInterface(compatibleInterface1));
  ASSERT_TRUE(converter.checkInterface(compatibleInterface2));
  ASSERT_FALSE(converter.checkInterface(incompatibleInterface1));
  ASSERT_FALSE(converter.checkInterface(incompatibleInterface2));
  ASSERT_FALSE(converter.checkInterface(incompatibleInterface3));
  ASSERT_FALSE(converter.checkInterface(incompatibleInterface4));
  ASSERT_FALSE(converter.checkInterface(incompatibleInterface5));
}

#endif
