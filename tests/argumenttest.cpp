#ifndef ARGUMENT_TEST_CPP
#define ARGUMENT_TEST_CPP

#include <argument.h>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>
#include <QSharedPointer>
#include <QString>

using namespace testing;

TEST(argumentTests, boolArgumentTest) {
  ASSERT_NO_THROW({
    ArgumentImplementation<bool> trueArgument(QJsonValue(true));
    ArgumentImplementation<bool> falseArgument(QJsonValue(false));
    ASSERT_EQ((*((bool*)trueArgument.getArgument().data())), true);
    ASSERT_EQ((*((bool*)falseArgument.getArgument().data())), false);
  });
  ASSERT_THROW({
    ArgumentImplementation<bool> trueArgument(QJsonValue("true"));
  }, QString);
  ASSERT_THROW({
      ArgumentImplementation<bool> falseArgument(QJsonValue("false"));
  }, QString);
}
#endif
