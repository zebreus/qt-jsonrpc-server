#ifndef CALL_MANAGER_TEST_CPP
#define CALL_MANAGER_TEST_CPP


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
#include <callmanager.h>
#include <message.h>

using namespace testing;
using namespace jsonrpc;

class CallManagerTests : public ::testing::Test {
 protected:
  void SetUp() override {
      target = new MockTarget();
      callManager = new CallManager(target);
  }

  CallManager* callManager;
  QObject* target;
  void TearDown() override {
      delete callManager;
      delete target;
  }
};

TEST_F(CallManagerTests, callWithIntsWorks) {
    QList<QJsonValue> arguments{QJsonValue(4), QJsonValue(13)};
    QString method("addNumbers");
    QJsonValue id = 5;
    QSharedPointer<Request> request(new Request(id, method, arguments));

    QSharedPointer<jsonrpc::Message> returnValue(nullptr);
    QCoreApplication::connect(callManager, &CallManager::respond, [&returnValue](QSharedPointer<jsonrpc::Message> value){
        returnValue.swap(value);
    });
    callManager->processRequest(request);

    QTime limit = QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < limit && returnValue == nullptr) {
      QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }

    ASSERT_NE(returnValue.get(), nullptr);
    ASSERT_EQ(((Response*)returnValue.get())->getResult().toDouble(), 17);
}

TEST_F(CallManagerTests, callWithInvalidParameterCountDoesNotWork) {
    QList<QJsonValue> arguments{QJsonValue(4), QJsonValue(13), QJsonValue(12)};
    QString method("addNumbers");
    QJsonValue id = 5;
    QSharedPointer<Request> request(new Request(id, method, arguments));

    QSharedPointer<jsonrpc::Message> returnValue(nullptr);
    QCoreApplication::connect(callManager, &CallManager::respond, [&returnValue](QSharedPointer<jsonrpc::Message> value){
        returnValue.swap(value);
    });
    ASSERT_NO_THROW({
        callManager->processRequest(request);
    });

    QTime limit = QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < limit && returnValue == nullptr) {
      QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }

    ASSERT_NE(returnValue.get(), nullptr);
    ASSERT_EQ(((Error*)returnValue.get())->getCode(), Error::MethodNotFound);
}

TEST_F(CallManagerTests, callWithInvalidParameterTypesDoesNotWork) {
    QList<QJsonValue> arguments{QJsonValue(4), QJsonValue("hallo")};
    QString method("addNumbers");
    QJsonValue id = 5;
    QSharedPointer<Request> request(new Request(id, method, arguments));

    QSharedPointer<jsonrpc::Message> returnValue(nullptr);
    QCoreApplication::connect(callManager, &CallManager::respond, [&returnValue](QSharedPointer<jsonrpc::Message> value){
        returnValue.swap(value);
    });
    ASSERT_NO_THROW({
        callManager->processRequest(request);
    });

    QTime limit = QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < limit && returnValue == nullptr) {
      QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }

    ASSERT_NE(returnValue.get(), nullptr);
    ASSERT_EQ(((Error*)returnValue.get())->getCode(), Error::InvalidParams);
}

#endif
