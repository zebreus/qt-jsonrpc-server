#ifndef METHOD_DESCRIPTION_TEST_CPP
#define METHOD_DESCRIPTION_TEST_CPP

#include <argument.h>
#include <callmanager.h>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <interfacedescription.h>
#include <message.h>
#include <methoddescription.h>
#include <server.h>

#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QSharedPointer>
#include <QSignalSpy>
#include <QTime>
#include <limits>

#include "mocktarget.hpp"

using namespace testing;
using namespace jsonrpc;

TEST(interfaceDescriptionTest, sameInterfacesAreEqual) {
  InterfaceDescription a("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}});
  InterfaceDescription b("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}});
  ASSERT_EQ(a, b);
  ASSERT_FALSE(a != b);
}

TEST(interfaceDescriptionTest, interfacesWithDifferentNamesAreUnqual) {
  InterfaceDescription a("nameXXX", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}});
  InterfaceDescription b("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}});
  ASSERT_NE(a, b);
  ASSERT_FALSE(a == b);
}

TEST(interfaceDescriptionTest, interfacesWithDifferentVersionsAreUnqual) {
  InterfaceDescription a("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}}, 1.0);
  InterfaceDescription b("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}}, 0.5);
  ASSERT_NE(a, b);
  ASSERT_FALSE(a == b);
}

TEST(interfaceDescriptionTest, interfacesWithDifferentSlotsAreUnqual) {
  InterfaceDescription a("name", {{"doSomething", {"bool", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}});
  InterfaceDescription b("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}});
  ASSERT_NE(a, b);
  ASSERT_FALSE(a == b);
}

TEST(interfaceDescriptionTest, interfacesWithDifferentSignalsAreUnqual) {
  InterfaceDescription a("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}});
  InterfaceDescription b("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"bool", "bool"}, "void"}});
  ASSERT_NE(a, b);
  ASSERT_FALSE(a == b);
}

TEST(interfaceDescriptionTest, interfacesWithDifferentSlotCountsAreUnqual) {
  InterfaceDescription a("name", {{"doSomething", {"int", "bool"}, "void"}, {}}, {{"signal", {"int", "bool"}, "void"}});
  InterfaceDescription b("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}});
  ASSERT_NE(a, b);
  ASSERT_FALSE(a == b);
}

TEST(interfaceDescriptionTest, interfacesWithDifferentSignalCountsAreUnqual) {
  InterfaceDescription a("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}, {}});
  InterfaceDescription b("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}});
  ASSERT_NE(a, b);
  ASSERT_FALSE(a == b);
}

TEST(interfaceDescriptionTest, defaultVersionIsZero) {
  InterfaceDescription a("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}}, 0);
  InterfaceDescription b("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}});
  ASSERT_EQ(a, b);
}

TEST(interfaceDescriptionTest, copyConstructorWorks) {
  InterfaceDescription a("name", {{"doSomething", {"int", "bool"}, "void"}}, {{"signal", {"int", "bool"}, "void"}});
  InterfaceDescription b(a);
  ASSERT_EQ(a, b);
}

TEST(interfaceDescriptionTest, defaultConstructorWorks) {
  InterfaceDescription a;
  InterfaceDescription b;
  ASSERT_EQ(a, b);
}

TEST(interfaceDescriptionTest, qMetaObjectBasedConstructorWorks) {
  QScopedPointer<MockTarget> mockTarget(new MockTarget());
  const QMetaObject* metaObject = mockTarget->metaObject();
  InterfaceDescription realDescription(*metaObject);
  InterfaceDescription expectedDescription("MockTarget",
                                           {
                                               {"addNumbers", {"int", "int"}, "int"},
                                               {"echoString", {"QString"}, "QString"},
                                               {"noParams", {}, "int"},
                                               {"emitSignalA", {}, "void"},
                                               {"emitSignalB", {}, "void"},
                                           },
                                           {{"signalA", {}, "void"}, {"signalB", {"QString"}, "void"}});

  ASSERT_EQ(realDescription, expectedDescription);
}

TEST(interfaceDescriptionTest, interfaceDescriptionIsRegisteredWithQtMetatypSystem) {
  int metaTypeId = QMetaType::fromType<InterfaceDescription>().id();
  ASSERT_NE(metaTypeId, QMetaType::UnknownType);
  ASSERT_TRUE(QMetaType::isRegistered(metaTypeId));
  ASSERT_EQ(metaTypeId, QMetaType::type("jsonrpc::InterfaceDescription"));
}

#endif
