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

//Proxy function, as Argument might get changed
template<typename T>
QSharedPointer<Argument> createArgument(const QJsonValue& value){
    return QSharedPointer<Argument>(new ArgumentImplementation<T>(value));
}

//Proxy function, as Argument might get changed
template<typename T>
T& getArgumentValue(const QSharedPointer<Argument>& argument){
    return *((T*)argument->getArgument().data());
}

TEST(argumentTests, boolArgumentTest) {
  ASSERT_NO_THROW({
    auto trueArgument = createArgument<bool>(QJsonValue(true));
    auto falseArgument = createArgument<bool>(QJsonValue(false));
    ASSERT_EQ(getArgumentValue<bool>(trueArgument), true);
    ASSERT_EQ(getArgumentValue<bool>(falseArgument), false);
  });
  ASSERT_THROW({
    createArgument<bool>(QJsonValue("true"));
  }, QString);
  ASSERT_THROW({
    createArgument<bool>(QJsonValue("false"));
  }, QString);
  ASSERT_THROW({
    createArgument<bool>(QJsonValue(0));
  }, QString);
  ASSERT_THROW({
    createArgument<bool>(QJsonValue::Undefined);
  }, QString);
}
#endif
