#ifndef CALL_TEST_CPP
#define CALL_TEST_CPP

#include "mocktarget.hpp"

#include <argument.h>
#include <call.h>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

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

using namespace testing;
using namespace jsonrpc;

TEST(callTests, callWithIntsWorks) {
  QList<QJsonValue> arguments{QJsonValue(4), QJsonValue(13)};
  MockTarget target;
  QString method("addNumbers");
  QScopedPointer<Call> call(nullptr);
  ASSERT_NO_THROW({ call.reset(new Call(&target, method, arguments)); });
  QJsonValue returnValue = QJsonValue::Undefined;
  QCoreApplication::connect(call.get(), &Call::onSuccess, [&returnValue](QJsonValue value) {
    returnValue = value;
  });
  call->invoke();

  QTime limit = QTime::currentTime().addMSecs(500);
  while(QTime::currentTime() < limit && returnValue == QJsonValue::Undefined) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
  }

  ASSERT_NE(returnValue, QJsonValue::Undefined);
  ASSERT_TRUE(returnValue.isDouble());
  ASSERT_EQ((int)returnValue.toDouble(), 17);
}

TEST(callTests, callWithStringsWorks) {
  QList<QJsonValue> arguments{QJsonValue("EDU")};
  MockTarget target;
  QString method("echoString");
  QScopedPointer<Call> call(nullptr);
  ASSERT_NO_THROW({ call.reset(new Call(&target, method, arguments)); });
  QJsonValue returnValue = QJsonValue::Undefined;
  QCoreApplication::connect(call.get(), &Call::onSuccess, [&returnValue](QJsonValue value) {
    returnValue = value;
  });
  call->invoke();

  QTime limit = QTime::currentTime().addMSecs(500);
  while(QTime::currentTime() < limit && returnValue == QJsonValue::Undefined) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
  }

  ASSERT_NE(returnValue, QJsonValue::Undefined);
  ASSERT_TRUE(returnValue.isString());
  ASSERT_EQ(returnValue.toString(), "EDU");
}

TEST(callTests, callWithoutParametersWorks) {
  QList<QJsonValue> arguments{};
  MockTarget target;
  QString method("noParams");
  QScopedPointer<Call> call(nullptr);
  ASSERT_NO_THROW({ call.reset(new Call(&target, method, arguments)); });
  QJsonValue returnValue = QJsonValue::Undefined;
  QCoreApplication::connect(call.get(), &Call::onSuccess, [&returnValue](QJsonValue value) {
    returnValue = value;
  });
  call->invoke();

  QTime limit = QTime::currentTime().addMSecs(500);
  while(QTime::currentTime() < limit && returnValue == QJsonValue::Undefined) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
  }

  ASSERT_NE(returnValue, QJsonValue::Undefined);
  ASSERT_TRUE(returnValue.isDouble());
  ASSERT_EQ((int)returnValue.toDouble(), 564);
}

TEST(callTests, callFailsWithWrongParameterTypes) {
  QList<QJsonValue> arguments{QJsonValue(76)};
  MockTarget target;
  QString method("echoString");
  QScopedPointer<Call> call(nullptr);
  ASSERT_THROW({ call.reset(new Call(&target, method, arguments)); }, exceptions::WrongArgumentType);
}

TEST(callTests, callFailsWithTooFewParameters) {
  QList<QJsonValue> arguments{QJsonValue(76)};
  MockTarget target;
  QString method("addNumbers");
  QScopedPointer<Call> call(nullptr);
  ASSERT_THROW({ call.reset(new Call(&target, method, arguments)); }, exceptions::WrongArgumentCount);
}

TEST(callTests, callFailsWithTooManyParameters) {
  QList<QJsonValue> arguments{QJsonValue(76), QJsonValue(76), QJsonValue(76)};
  MockTarget target;
  QString method("addNumbers");
  QScopedPointer<Call> call(nullptr);
  ASSERT_THROW({ call.reset(new Call(&target, method, arguments)); }, exceptions::WrongArgumentCount);
}

TEST(callTests, callFailsWithNonexistanMethodName) {
  QList<QJsonValue> arguments{QJsonValue(76), QJsonValue(76), QJsonValue(76)};
  MockTarget target;
  QString method("nope");
  QScopedPointer<Call> call(nullptr);
  ASSERT_THROW({ call.reset(new Call(&target, method, arguments)); }, exceptions::UnknownMethodName);
}

TEST(callTests, callOnSuperObjectWorks) {
  QList<QJsonValue> arguments{};
  SuperMock target;
  QString method("superSlot");
  QScopedPointer<Call> call(nullptr);
  ASSERT_NO_THROW({ call.reset(new Call(&target, method, arguments)); });
}

TEST(callTests, callOnSuperMethodFails) {
  QList<QJsonValue> arguments{};
  MockTarget target;
  QString method("superSlot");
  QScopedPointer<Call> call(nullptr);
  ASSERT_THROW({ call.reset(new Call(&target, method, arguments)); }, exceptions::UnknownMethodName);
}

TEST(callTests, callOnPrivateMethodFails) {
  QList<QJsonValue> arguments{};
  MockTarget target;
  QString method("privateSlot");
  QScopedPointer<Call> call(nullptr);
  ASSERT_THROW({ call.reset(new Call(&target, method, arguments)); }, exceptions::UnknownMethodName);
}

#endif
