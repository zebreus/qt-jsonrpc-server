#ifndef SERVER_TEST_CPP
#define SERVER_TEST_CPP

#include <argument.h>
#include <callmanager.h>
#define private public
#include <connection.h>
#undef private
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <message.h>
#include <request.h>
#include <response.h>
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
#include <thread>

#include "mocktarget.hpp"

using namespace testing;
using namespace jsonrpc;

class ConnectionTests: public ::testing::Test {
 protected:
  void SetUp() override {
    target = new MockTarget(5);
    connection.reset(new Connection(target));
    target->setParent(connection.get());

    messageProcessor.reset(new MessageProcessor());
    QCoreApplication::connect(
        messageProcessor.get(),
        &MessageProcessor::receivedRequest,
        messageProcessor.get(),
        [this](const QSharedPointer<jsonrpc::Request>& request) {
          receivedRequests.append(request);
        },
        Qt::DirectConnection);
    QCoreApplication::connect(
        messageProcessor.get(),
        &MessageProcessor::receivedResponse,
        messageProcessor.get(),
        [this](const QSharedPointer<jsonrpc::Response>& request) {
          receivedResponses.append(request);
        },
        Qt::DirectConnection);
    QCoreApplication::connect(
        messageProcessor.get(),
        &MessageProcessor::receivedError,
        messageProcessor.get(),
        [this](const QSharedPointer<jsonrpc::Error>& request) {
          receivedErrors.append(request);
        },
        Qt::DirectConnection);
    QCoreApplication::connect(connection.get(),
                              &Connection::sendMessage,
                              messageProcessor.get(),
                              &MessageProcessor::receiveMessage,
                              Qt::DirectConnection);
  }

  void sendMessageToConnection(const QString& message) {
    connection->receiveMessage(message);
  }

  void sendMessageToConnection(const QJsonObject& message) {
    QJsonDocument doc(message);
    sendMessageToConnection(doc.toJson());
  }

  void sendMessageToConnection(const QSharedPointer<jsonrpc::Message>& message) {
    sendMessageToConnection(message->toJson());
  }

  QString getLastMessageStringFromConnection() {
    return receivedMessages.back();
  }

  QSharedPointer<jsonrpc::Request> getLastRequest() {
    if(receivedRequests.size() > 0) {
      return receivedRequests.last();
    } else {
      return nullptr;
    }
  }

  QSharedPointer<jsonrpc::Response> getLastResponse() {
    if(receivedResponses.size() > 0) {
      return receivedResponses.last();
    } else {
      return nullptr;
    }
  }
  QSharedPointer<jsonrpc::Error> getLastError() {
    if(receivedErrors.size() > 0) {
      return receivedErrors.last();
    } else {
      return nullptr;
    }
  }

  void processEvents(int millis, auto stopCondition = std::true_type()) {
    QTime limit = QTime::currentTime().addMSecs(millis);
    while(QTime::currentTime() < limit && stopCondition()) {
      QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }
  }

  MockTarget* target;
  QScopedPointer<Connection> connection;
  QScopedPointer<MessageProcessor> messageProcessor;

  QList<QString> receivedMessages;
  QList<QSharedPointer<jsonrpc::Request>> receivedRequests;
  QList<QSharedPointer<jsonrpc::Response>> receivedResponses;
  QList<QSharedPointer<jsonrpc::Error>> receivedErrors;

  void TearDown() override {}
};

TEST_F(ConnectionTests, connectionRespondsWithCorrectId) {
  sendMessageToConnection("{\"jsonrpc\":\"2.0\",\"method\":\"echoString\",\"params\":[\"stringd\"],\"id\":\"3\"}");
  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });
  ASSERT_NE(receivedResponses.size(), 0);
  ASSERT_NE(getLastResponse(), nullptr);
  ASSERT_EQ(getLastResponse()->getId(), QJsonValue("3"));
}

#endif
