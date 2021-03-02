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

using exceptionType = QString;

TEST(argumentTests, boolArgumentTest) {
  ASSERT_NO_THROW({
    auto trueArgument = createArgument<bool>(QJsonValue(true));
    auto falseArgument = createArgument<bool>(QJsonValue(false));
    ASSERT_EQ(getArgumentValue<bool>(trueArgument), true);
    ASSERT_EQ(getArgumentValue<bool>(falseArgument), false);
  });
  ASSERT_THROW({
    createArgument<bool>(QJsonValue("true"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<bool>(QJsonValue("false"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<bool>(QJsonValue(0));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<bool>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, intArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_EQ(getArgumentValue<int>(createArgument<int>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<int>(createArgument<int>(QJsonValue(5))), 5);
    ASSERT_EQ(getArgumentValue<int>(createArgument<int>(QJsonValue(INT_MAX))), INT_MAX);
                          ASSERT_EQ(getArgumentValue<int>(createArgument<int>(QJsonValue(INT_MIN))), INT_MIN);

  });
  ASSERT_THROW({
    createArgument<int>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<int>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<int>(QJsonValue(20ll + INT_MAX));
  }, exceptionType);
}
#endif
