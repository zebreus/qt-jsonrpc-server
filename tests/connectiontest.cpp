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

  processEvents(250, [this]() {
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

TEST_F(ConnectionTests, connectionRespondWithCorrectValue) {
  QSharedPointer<Request> addRequest(new Request(39, "addNumbers", {-18, 6000000}));

  sendMessageToConnection(addRequest);

  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_EQ(receivedResponses.size(), 1);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);
  QSharedPointer<Response> response = getLastResponse();
  ASSERT_NE(response, nullptr);
  ASSERT_EQ(response->getId(), QJsonValue(39));
  ASSERT_EQ(response->getResult(), QJsonValue(6000000 - 18));
}

TEST_F(ConnectionTests, connectionFailsOnWrongArgumentCount) {
  QSharedPointer<Request> addRequest1(new Request(39, "addNumbers", {-18, 6000000, 20}));
  QSharedPointer<Request> addRequest2(new Request(40, "addNumbers", {-18}));
  QSharedPointer<Request> addRequest3(new Request(41, "addNumbers", {}));
  QSharedPointer<Request> addRequest4(new Request(42, "addNumbers", {40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40}));

  sendMessageToConnection(addRequest1);
  sendMessageToConnection(addRequest2);
  sendMessageToConnection(addRequest3);
  sendMessageToConnection(addRequest4);

  processEvents(500, [this]() {
    return receivedErrors.size() == 4;
  });

  EXPECT_EQ(receivedResponses.size(), 0);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 4);
  for(QSharedPointer<Error> error : receivedErrors) {
    EXPECT_EQ(error->getCode(), Error::MethodNotFound);
  }
}

TEST_F(ConnectionTests, connectionFailsOnWrongArgumentTypes) {
  QSharedPointer<Request> addRequest1(new Request(39, "addNumbers", {5, "four"}));
  QSharedPointer<Request> addRequest2(new Request(40, "addNumbers", {"five", 4}));
  QSharedPointer<Request> addRequest3(new Request(41, "addNumbers", {"sadfa", "sfd"}));
  QSharedPointer<Request> addRequest4(new Request(42, "addNumbers", {5.5, QJsonValue::Undefined}));

  sendMessageToConnection(addRequest1);
  sendMessageToConnection(addRequest2);
  sendMessageToConnection(addRequest3);
  sendMessageToConnection(addRequest4);

  processEvents(500, [this]() {
    return receivedErrors.size() != 4;
  });

  EXPECT_EQ(receivedResponses.size(), 0);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 4);
  for(QSharedPointer<Error> error : receivedErrors) {
    EXPECT_EQ(error->getCode(), Error::InvalidParams);
  }
}

TEST_F(ConnectionTests, connectionFailsOnNonexistantMethod) {
  QSharedPointer<Request> addRequest1(new Request(39, "addNumbers ", {5, 5}));
  QSharedPointer<Request> addRequest2(new Request(40, " addNumbers", {5, 5}));
  QSharedPointer<Request> addRequest3(new Request(41, "_addNumbers", {5, 5}));
  QSharedPointer<Request> addRequest4(new Request(42, "äddNumbers", {5, 5}));

  sendMessageToConnection(addRequest1);
  sendMessageToConnection(addRequest2);
  sendMessageToConnection(addRequest3);
  sendMessageToConnection(addRequest4);

  processEvents(500, [this]() {
    return receivedErrors.size() != 4;
  });

  EXPECT_EQ(receivedResponses.size(), 0);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 4);
  for(QSharedPointer<Error> error : receivedErrors) {
    EXPECT_EQ(error->getCode(), Error::MethodNotFound);
  }
}

TEST_F(ConnectionTests, connectionVoidReturningMethodRespondsWithNullResult) {
  QSharedPointer<Request> voidReturningRequest(new Request(39, "emitSignalA", {}));

  sendMessageToConnection(voidReturningRequest);

  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_EQ(receivedResponses.size(), 1);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);
  QSharedPointer<Response> response = getLastResponse();
  ASSERT_NE(response, nullptr);
  ASSERT_EQ(response->getId(), QJsonValue(39));
  ASSERT_EQ(response->getResult(), QJsonValue::Null);
}

TEST_F(ConnectionTests, connectionWorksWithNumberId) {
  QJsonValue id(34);
  QSharedPointer<Request> request(new Request(id, "echoString", {"string"}));

  sendMessageToConnection(request);

  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_EQ(receivedResponses.size(), 1);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);
  ASSERT_NE(getLastResponse(), nullptr);
  ASSERT_EQ(getLastResponse()->getId(), id);
}

TEST_F(ConnectionTests, connectionWorksWithStringId) {
  QJsonValue id("cooleid");
  QSharedPointer<Request> request(new Request(id, "echoString", {"string"}));

  sendMessageToConnection(request);

  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_EQ(receivedResponses.size(), 1);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);
  ASSERT_NE(getLastResponse(), nullptr);
  ASSERT_EQ(getLastResponse()->getId(), id);
}

TEST_F(ConnectionTests, connectionWorksWithNullId) {
  QJsonValue id(QJsonValue::Null);
  QSharedPointer<Request> request(new Request(id, "echoString", {"string"}));

  sendMessageToConnection(request);

  processEvents(500, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_EQ(receivedResponses.size(), 1);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);
  ASSERT_NE(getLastResponse(), nullptr);
  ASSERT_EQ(getLastResponse()->getId(), id);
}

TEST_F(ConnectionTests, connectionTakesUndefinedIdAsNotification) {
  QJsonValue id(QJsonValue::Undefined);
  QSharedPointer<Request> request(new Request(id, "echoString", {"string"}));

  sendMessageToConnection(request);

  processEvents(250, [this]() {
    return receivedResponses.size() == 0;
  });

  EXPECT_EQ(receivedResponses.size(), 0);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 0);
}

TEST_F(ConnectionTests, connectionDoesNotWorkWithArrayOrObjectIds) {
  QString requestTemplate = "{\"jsonrpc\":\"2.0\",\"method\":\"echoString\",\"params\":[\"stringd\"],\"id\":%1}";
  QString requestNumberId = requestTemplate.arg("1");
  QString requestEmptyArrayId = requestTemplate.arg("[]");
  QString requestArrayId = requestTemplate.arg("[45]");
  QString requestEmptyObjectId = requestTemplate.arg("{}");
  QString requestObjectId = requestTemplate.arg("{\"number\":5}");
  sendMessageToConnection(requestNumberId);
  sendMessageToConnection(requestEmptyArrayId);
  sendMessageToConnection(requestArrayId);
  sendMessageToConnection(requestEmptyObjectId);
  sendMessageToConnection(requestObjectId);

  processEvents(500, [this]() {
    return (receivedErrors.size() != 4 && receivedResponses.size() != 1);
  });

  EXPECT_EQ(receivedResponses.size(), 1);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 4);

  for(const QSharedPointer<Error>& error : receivedErrors) {
    EXPECT_TRUE((error->getCode() == Error::InvalidMessage || error->getCode() == Error::InvalidRequest));
  }
}

TEST_F(ConnectionTests, connectionFailsWithParseErrorOnNullMessage) {
  QString request = "null";
  sendMessageToConnection(request);

  processEvents(500, [this]() {
    return (receivedErrors.size() != 1);
  });

  EXPECT_EQ(receivedResponses.size(), 0);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), 1);
  QSharedPointer<Error> error = getLastError();
  ASSERT_NE(error, nullptr);
  ASSERT_EQ(error->getCode(), Error::ParseError);
}

TEST_F(ConnectionTests, connectionFailsWithParseErrorOnRandomString) {
  QList<QString> requests = {"undefined", "98", "\x12\x00\xa4\x00\x00\x99", "test ꨑ", "{dsfsda}", "{", "}", "[sadfsda]", "[", "]"};
  for(const QString& request : requests) {
    sendMessageToConnection(request);
  }

  processEvents(500, [this, &requests]() {
    return (receivedErrors.size() != requests.size());
  });

  EXPECT_EQ(receivedResponses.size(), 0);
  EXPECT_EQ(receivedRequests.size(), 0);
  EXPECT_EQ(receivedErrors.size(), requests.size());
  for(const QSharedPointer<Error>& error : receivedErrors) {
    ASSERT_NE(error, nullptr);
    ASSERT_EQ(error->getCode(), Error::ParseError);
  }
}

#endif
