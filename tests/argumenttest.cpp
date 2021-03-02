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
#include <limits>

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
  ASSERT_THROW({
    createArgument<bool>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, unsignedIntArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_EQ(getArgumentValue<unsigned int>(createArgument<unsigned int>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<unsigned int>(createArgument<unsigned int>(QJsonValue(5))), 5);
    ASSERT_EQ(getArgumentValue<unsigned int>(createArgument<unsigned int>(QJsonValue(UINT_MAX +0ll))), UINT_MAX);
  });
  ASSERT_THROW({
    createArgument<unsigned int>(QJsonValue(-1));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned int>(QJsonValue(UINT_MAX+1ll));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned int>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned int>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned int>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, doubleArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_EQ(getArgumentValue<double>(createArgument<double>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<double>(createArgument<double>(QJsonValue(5))), 5);
    ASSERT_EQ(getArgumentValue<double>(createArgument<double>(QJsonValue(DBL_MIN))), DBL_MIN);
    ASSERT_EQ(getArgumentValue<double>(createArgument<double>(QJsonValue(DBL_MIN*2))), DBL_MIN*2);
    ASSERT_EQ(getArgumentValue<double>(createArgument<double>(QJsonValue(DBL_MAX))), DBL_MAX);
    ASSERT_EQ(getArgumentValue<double>(createArgument<double>(QJsonValue(-DBL_MAX))), -DBL_MAX);
    ASSERT_TRUE(std::isnan(getArgumentValue<double>(createArgument<double>(QJsonValue(std::numeric_limits<double>::quiet_NaN())))));
    ASSERT_TRUE(std::isnan(getArgumentValue<double>(createArgument<double>(QJsonValue(std::numeric_limits<double>::signaling_NaN())))));
    ASSERT_EQ(getArgumentValue<double>(createArgument<double>(QJsonValue(std::numeric_limits<double>::infinity()))), std::numeric_limits<double>::infinity());
  });
  ASSERT_THROW({
    createArgument<double>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<double>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<double>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, floatArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_EQ(getArgumentValue<float>(createArgument<float>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<float>(createArgument<float>(QJsonValue(5))), 5);
    ASSERT_EQ(getArgumentValue<float>(createArgument<float>(QJsonValue(FLT_MIN))), FLT_MIN);
    ASSERT_EQ(getArgumentValue<float>(createArgument<float>(QJsonValue(FLT_MIN*2))), FLT_MIN*2);
    ASSERT_NE(getArgumentValue<float>(createArgument<float>(QJsonValue(DBL_MIN*2))), DBL_MIN*2);
    ASSERT_EQ(getArgumentValue<float>(createArgument<float>(QJsonValue(FLT_MAX))), FLT_MAX);
    ASSERT_EQ(getArgumentValue<float>(createArgument<float>(QJsonValue(-FLT_MAX))), -FLT_MAX);
    ASSERT_TRUE(std::isnan(getArgumentValue<float>(createArgument<float>(QJsonValue(std::numeric_limits<float>::quiet_NaN())))));
    ASSERT_TRUE(std::isnan(getArgumentValue<float>(createArgument<float>(QJsonValue(std::numeric_limits<float>::signaling_NaN())))));
    ASSERT_EQ(getArgumentValue<float>(createArgument<float>(QJsonValue(std::numeric_limits<float>::infinity()))), std::numeric_limits<float>::infinity());
  });
  ASSERT_THROW({
    createArgument<float>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<float>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<float>(QJsonValue::Undefined);
  }, exceptionType);
}
#endif
