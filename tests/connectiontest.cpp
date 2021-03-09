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
    QCoreApplication::connect(connection.get(), &Connection::sendMessage, messageProcessor.get(), [this](const QString& string) {
      receivedMessages.append(string);
    });
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
    if(receivedMessages.size() > 0) {
      return receivedMessages.last();
    } else {
      return "";
    }
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

TEST_F(ConnectionTests, connectionRespondsToRequest) {
  sendMessageToConnection("{\"jsonrpc\":\"2.0\",\"method\":\"echoString\",\"params\":[\"stringd\"],\"id\":\"3\"}");
  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });
  EXPECT_NE(receivedResponses.size(), 0);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);
}

TEST_F(ConnectionTests, connectionRespondsWithCorrectId) {
  QJsonValue id(34);
  QString method = "echoString";
  QList<QJsonValue> arguments = {"String"};
  QSharedPointer<Request> request(new Request(id, method, arguments));

  sendMessageToConnection(request);

  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_NE(receivedResponses.size(), 0);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);
  ASSERT_NE(getLastResponse(), nullptr);
  ASSERT_EQ(getLastResponse()->getId(), QJsonValue(34));
}

TEST_F(ConnectionTests, connectionDoesNotRespondToNotification) {
  QString method = "echoString";
  QList<QJsonValue> arguments = {"String"};
  QSharedPointer<Request> request(new Request(method, arguments));

  sendMessageToConnection(request);

  processEvents(400, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_EQ(receivedResponses.size(), 0);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);
}

TEST_F(ConnectionTests, connectionDoesNotEmitSignalBeforeActivation) {
  QJsonValue id(34);
  QString method = "emitSignalA";
  QList<QJsonValue> arguments = {};
  QSharedPointer<Request> request(new Request(id, method, arguments));

  sendMessageToConnection(request);

  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_EQ(receivedResponses.size(), 1);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);
}

TEST_F(ConnectionTests, connectionDoesNotFailOnActivate) {
  QJsonValue id(34);
  QString method = "rpc.qt.activate";
  QList<QJsonValue> arguments = {};
  QSharedPointer<Request> request(new Request(id, method, arguments));

  sendMessageToConnection(request);

  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_EQ(receivedResponses.size(), 1);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);
}

TEST_F(ConnectionTests, connectionDoesNotFailOnDeactivate) {
  QJsonValue id(34);
  QString method = "rpc.qt.deactivate";
  QList<QJsonValue> arguments = {};
  QSharedPointer<Request> request(new Request(id, method, arguments));

  sendMessageToConnection(request);

  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_EQ(receivedResponses.size(), 1);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);
}

TEST_F(ConnectionTests, connectionDoesEmitSignalAfterActivation) {
  QSharedPointer<Request> emitRequest(new Request(34, "emitSignalA", {}));

  QSharedPointer<Request> activateRequest(new Request(35, "rpc.qt.activate", {}));

  sendMessageToConnection(activateRequest);

  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_EQ(receivedResponses.size(), 1);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);

  sendMessageToConnection(emitRequest);

  processEvents(500, [this]() {
    return !(receivedResponses.size() == 2 && receivedRequests.size() == 1);
  });

  EXPECT_EQ(receivedResponses.size(), 2);
  EXPECT_EQ(receivedRequests.size(), 1);
  EXPECT_EQ(receivedErrors.size(), 0);
}

TEST_F(ConnectionTests, connectionDoesNotEmitSignalAfterDeactivation) {
  QSharedPointer<Request> emitRequest(new Request(34, "emitSignalA", {}));

  QSharedPointer<Request> activateRequest(new Request(35, "rpc.qt.activate", {}));

  QSharedPointer<Request> deactivateRequest(new Request(35, "rpc.qt.deactivate", {}));

  sendMessageToConnection(activateRequest);

  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_EQ(receivedResponses.size(), 1);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);

  sendMessageToConnection(emitRequest);

  processEvents(500, [this]() {
    return !(receivedResponses.size() == 2 && receivedRequests.size() == 1);
  });

  EXPECT_EQ(receivedResponses.size(), 2);
  EXPECT_EQ(receivedRequests.size(), 1);
  EXPECT_EQ(receivedErrors.size(), 0);

  sendMessageToConnection(deactivateRequest);

  processEvents(500, [this]() {
    return !(receivedResponses.size() == 3 && receivedRequests.size() == 1);
  });

  EXPECT_EQ(receivedResponses.size(), 3);
  EXPECT_EQ(receivedRequests.size(), 1);
  EXPECT_EQ(receivedErrors.size(), 0);

  sendMessageToConnection(emitRequest);

  processEvents(500, [this]() {
    return !(receivedResponses.size() == 4 && receivedRequests.size() == 1);
  });

  EXPECT_EQ(receivedResponses.size(), 4);
  EXPECT_EQ(receivedRequests.size(), 1);
  EXPECT_EQ(receivedErrors.size(), 0);
}

#endif
