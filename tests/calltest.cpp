#ifndef ARGUMENT_TEST_CPP
#define ARGUMENT_TEST_CPP


#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <QTime>
#include <QCoreApplication>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QSharedPointer>
#include <QSignalSpy>
#include <limits>
#include <argument.h>
#include "calltest.hpp"
#include <call.h>

using namespace testing;
using namespace jsonrpc;

TEST(callTests, callWithIntsWorks) {
    QList<QJsonValue> arguments{QJsonValue(4), QJsonValue(13)};
    MockTarget target;
    QString method("addNumbers");
    QScopedPointer<Call> call(nullptr);
    ASSERT_NO_THROW({
        call.reset(new Call(&target, method, arguments));
    });
    QJsonValue returnValue = QJsonValue::Undefined;
    QCoreApplication::connect(call.get(), &Call::onSuccess, [&returnValue](QJsonValue value){
        returnValue = value;
    });
    call->invoke();

    QTime limit = QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < limit && returnValue == QJsonValue::Undefined) {
      QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }

    ASSERT_NE(returnValue, QJsonValue::Undefined);
    ASSERT_TRUE(returnValue.isDouble());
    ASSERT_EQ((int)returnValue.toDouble(),17);
}

TEST(callTests, callWithStringsWorks) {
    QList<QJsonValue> arguments{QJsonValue("EDU")};
    MockTarget target;
    QString method("echoString");
    QScopedPointer<Call> call(nullptr);
    ASSERT_NO_THROW({
        call.reset(new Call(&target, method, arguments));
    });
    QJsonValue returnValue = QJsonValue::Undefined;
    QCoreApplication::connect(call.get(), &Call::onSuccess, [&returnValue](QJsonValue value){
        returnValue = value;
    });
    call->invoke();

    QTime limit = QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < limit && returnValue == QJsonValue::Undefined) {
      QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }

    ASSERT_NE(returnValue, QJsonValue::Undefined);
    ASSERT_TRUE(returnValue.isString());
    ASSERT_EQ(returnValue.toString(),"EDU");
}

TEST(callTests, callWithoutParametersWorks) {
    QList<QJsonValue> arguments{};
    MockTarget target;
    QString method("noParams");
    QScopedPointer<Call> call(nullptr);
    ASSERT_NO_THROW({
        call.reset(new Call(&target, method, arguments));
    });
    QJsonValue returnValue = QJsonValue::Undefined;
    QCoreApplication::connect(call.get(), &Call::onSuccess, [&returnValue](QJsonValue value){
        returnValue = value;
    });
    call->invoke();

    QTime limit = QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < limit && returnValue == QJsonValue::Undefined) {
      QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }

    ASSERT_NE(returnValue, QJsonValue::Undefined);
    ASSERT_TRUE(returnValue.isDouble());
    ASSERT_EQ((int)returnValue.toDouble(),564);
}

TEST(callTests, callFailsWithWrongParameterTypes) {
    QList<QJsonValue> arguments{QJsonValue(76)};
    MockTarget target;
    QString method("echoString");
    QScopedPointer<Call> call(nullptr);
    ASSERT_THROW({
        call.reset(new Call(&target, method, arguments));
    }, QString);
}

TEST(callTests, callFailsWithTooFewParameters) {
    QList<QJsonValue> arguments{QJsonValue(76)};
    MockTarget target;
    QString method("addNumbers");
    QScopedPointer<Call> call(nullptr);
    ASSERT_THROW({
        call.reset(new Call(&target, method, arguments));
    }, Error);
}

TEST(callTests, callFailsWithTooManyParameters) {
    QList<QJsonValue> arguments{QJsonValue(76), QJsonValue(76), QJsonValue(76)};
    MockTarget target;
    QString method("addNumbers");
    QScopedPointer<Call> call(nullptr);
    ASSERT_THROW({
        call.reset(new Call(&target, method, arguments));
    }, Error);
}

TEST(callTests, callFailsWithNonexistanMethodName) {
    QList<QJsonValue> arguments{QJsonValue(76), QJsonValue(76), QJsonValue(76)};
    MockTarget target;
    QString method("nope");
    QScopedPointer<Call> call(nullptr);
    ASSERT_THROW({
        call.reset(new Call(&target, method, arguments));
    }, Error);
}

TEST(callTests, callOnSuperObjectWorks) {
    QList<QJsonValue> arguments{};
    SuperMock target;
    QString method("superSlot");
    QScopedPointer<Call> call(nullptr);
    ASSERT_NO_THROW({
        call.reset(new Call(&target, method, arguments));
    });
}

TEST(callTests, callOnSuperMethodFails) {
    QList<QJsonValue> arguments{};
    MockTarget target;
    QString method("superSlot");
    QScopedPointer<Call> call(nullptr);
    ASSERT_THROW({
        call.reset(new Call(&target, method, arguments));
    }, Error);
}

#endif
