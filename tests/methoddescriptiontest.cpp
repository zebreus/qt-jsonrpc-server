#ifndef METHOD_DESCRIPTION_TEST_CPP
#define METHOD_DESCRIPTION_TEST_CPP

#include <argument.h>
#include <callmanager.h>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
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

TEST(methodDescriptionTest, sameSignaturesAreEqual) {
  MethodDescription a("name", {"int", "bool"}, "void");
  MethodDescription b("name", {"int", "bool"}, "void");
  ASSERT_EQ(a, b);
  ASSERT_FALSE(a != b);
}

TEST(methodDescriptionTest, methodsWithDifferentNamesAreNotEqual) {
  MethodDescription a("name", {"int", "bool"}, "void");
  MethodDescription b("nameXXX", {"int", "bool"}, "void");
  ASSERT_NE(a, b);
  ASSERT_FALSE(a == b);
}

TEST(methodDescriptionTest, methodsWithDifferentReturnTypesAreNotEqual) {
  MethodDescription a("name", {"int", "bool"}, "void");
  MethodDescription b("name", {"int", "bool"}, "int");
  ASSERT_NE(a, b);
  ASSERT_FALSE(a == b);
}

TEST(methodDescriptionTest, methodsWithDifferentParameterTypesAreNotEqual) {
  MethodDescription a("name", {"int", "int"}, "void");
  MethodDescription b("name", {"int", "bool"}, "void");
  ASSERT_NE(a, b);
  ASSERT_FALSE(a == b);
}

TEST(methodDescriptionTest, methodsWithDifferentParameterCountAreNotEqual) {
  MethodDescription a("name", {"int", "bool"}, "void");
  MethodDescription b("name", {"int", "bool", "int"}, "void");
  ASSERT_NE(a, b);
  ASSERT_FALSE(a == b);
}

TEST(methodDescriptionTest, defaultReturnTypeIsVoid) {
  MethodDescription a("name", {"int", "bool"}, "void");
  MethodDescription b("name", QList<QString>{"int", "bool"});
  ASSERT_EQ(a, b);
}

TEST(methodDescriptionTest, copyConstructorWorks) {
  MethodDescription a("name", {"int", "bool"}, "void");
  MethodDescription b(a);
  ASSERT_EQ(a, b);
}

TEST(methodDescriptionTest, defaultConstructorWorks) {
  MethodDescription a;
  MethodDescription b("", {}, "");
  ASSERT_EQ(a, b);
}

TEST(methodDescriptionTest, typeNameBasedConstructorWorks) {
  QString name = "myname";
  QString param1 = "int";
  QString param2 = "double";
  QString returnType = "QChar";
  MethodDescription a(name, {param1, param2}, returnType);
  ASSERT_EQ(name, a.getName());
  ASSERT_EQ(2, a.getParameterTypes().size());
  ASSERT_EQ(param1, a.getParameterTypes()[0]);
  ASSERT_EQ(param2, a.getParameterTypes()[1]);
  ASSERT_EQ(returnType, a.getReturnType());
}

TEST(methodDescriptionTest, typeIdBasedConstructorWorks) {
  QString name = "myname";
  int param1 = QMetaType::Int;
  int param2 = QMetaType::Bool;
  int returnType = QMetaType::Void;
  MethodDescription a(name, {param1, param2}, returnType);
  ASSERT_EQ(name, a.getName());
  ASSERT_EQ(2, a.getParameterTypeIds().size());
  ASSERT_EQ(param1, a.getParameterTypeIds().at(0));
  ASSERT_EQ(param2, a.getParameterTypeIds().at(1));
  ASSERT_EQ(returnType, a.getReturnTypeId());
  ASSERT_EQ(QMetaType::typeName(param1), a.getParameterTypes().at(0));
  ASSERT_EQ(QMetaType::typeName(param2), a.getParameterTypes().at(1));
  ASSERT_EQ(QMetaType::typeName(returnType), a.getReturnType());
}

TEST(methodDescriptionTest, qMetaMethodBasedConstructorWorks) {
  MethodDescription myAddNumbers("addNumbers", {"int", "int"}, "int");
  QScopedPointer<MockTarget> mockTarget(new MockTarget());
  const QMetaObject* metaObject = mockTarget->metaObject();
  int methodIndex = metaObject->indexOfSlot(QMetaObject::normalizedSignature("addNumbers(int,int)"));
  QMetaMethod addNumbersMeta = metaObject->method(methodIndex);

  MethodDescription realAddNumbers(addNumbersMeta);
  MethodDescription expectedAddNumbers("addNumbers", {"int", "int"}, "int");
  ASSERT_EQ(realAddNumbers, expectedAddNumbers);
}

TEST(methodDescriptionTest, methodDescriptionIsRegisteredWithQtMetatypSystem) {
  int metaTypeId = QMetaType::fromType<MethodDescription>().id();
  ASSERT_NE(metaTypeId, QMetaType::UnknownType);
  ASSERT_TRUE(QMetaType::isRegistered(metaTypeId));
  ASSERT_EQ(metaTypeId, QMetaType::type("jsonrpc::MethodDescription"));
}

#endif
