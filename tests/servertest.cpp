#ifndef SERVER_TEST_CPP
#define SERVER_TEST_CPP

#include <argument.h>
#include <callmanager.h>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <message.h>
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

class ServerTests: public ::testing::Test {
 protected:
  void SetUp() override {
    server.reset(new jsonrpc::Server<MockTarget>(56730));
    server->setConstructorArguments(56);
    server->startListening();
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
  }

  QScopedPointer<Server<MockTarget>> server;
  void TearDown() override {}
};

TEST_F(ServerTests, serverRespondsSomething) {
  QWebSocket webSocket;

  bool connected = false;
  bool lostConnection = false;
  bool receivedResponse = false;
  QObject::connect(&webSocket, &QWebSocket::connected, [&connected]() {
    connected = true;
  });
  QObject::connect(&webSocket, &QWebSocket::disconnected, [&lostConnection]() {
    lostConnection = true;
  });
  QObject::connect(&webSocket, &QWebSocket::textMessageReceived, [&receivedResponse]() {
    receivedResponse = true;
  });
  QObject::connect(&webSocket, &QWebSocket::binaryMessageReceived, [&receivedResponse]() {
    receivedResponse = true;
  });
  QObject::connect(&webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), [&lostConnection]() {
    lostConnection = true;
  });

  webSocket.open(QUrl("ws://localhost:56730"));
  QTime limit1 = QTime::currentTime().addMSecs(500);
  while(QTime::currentTime() < limit1 && connected == false) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
  }
  ASSERT_TRUE(webSocket.isValid());
  webSocket.sendTextMessage("{\"jsonrpc\":\"2.0\",\"method\":\"echoString\",\"params\":[\"stringd\"],\"id\":\"3\"}");
  QTime limit = QTime::currentTime().addMSecs(500);
  while(QTime::currentTime() < limit && lostConnection == false && receivedResponse == false) {
    QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
  }
  ASSERT_TRUE(receivedResponse);
}

#endif
