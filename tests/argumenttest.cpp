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

//Convert to argument and back
template<typename T>
QJsonValue argumentValueToJson(const T& value){
    QScopedPointer<Argument> argument(new ArgumentImplementation<T>());
    *((T*)argument->getArgument().data()) = value;
    return argument->getJson();
}

template<typename T>
T argumentJsonToValue(const QJsonValue& json){
    QScopedPointer<Argument> argument(new ArgumentImplementation<T>(json));
    return *((T*)argument->getArgument().data());
}

template<typename T>
bool roundtripWorks(const T& value){
    return (value == argumentJsonToValue<T>(argumentValueToJson<T>(value)));
}

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

TEST(argumentTests, argumentCreateWithUndefinedWorks) {
    ASSERT_NO_THROW({
      ASSERT_EQ(QSharedPointer<Argument>(Argument::create(QMetaType::Int))->getJson(), 0);
      ASSERT_EQ(QSharedPointer<Argument>(Argument::create(QMetaType::Int, QJsonValue::Undefined))->getJson(), 0);
    });
}

//Integer types

TEST(argumentTests, shortArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<short>(0));
    ASSERT_TRUE(roundtripWorks<short>(5));
    ASSERT_TRUE(roundtripWorks<short>(std::numeric_limits<short>::min()));
    ASSERT_TRUE(roundtripWorks<short>(std::numeric_limits<short>::max()));
  });
  ASSERT_THROW({
    createArgument<short>(QJsonValue(std::numeric_limits<short>::max() + 1ll));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<short>(QJsonValue(std::numeric_limits<short>::min() - 1ll));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<short>(QJsonValue(5.5f));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<short>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<short>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<short>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, unsignedShortArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<unsigned short>(0));
    ASSERT_TRUE(roundtripWorks<unsigned short>(5));
    ASSERT_TRUE(roundtripWorks<unsigned short>(std::numeric_limits<unsigned short>::lowest()));
    ASSERT_TRUE(roundtripWorks<unsigned short>(std::numeric_limits<unsigned short>::min()));
    ASSERT_TRUE(roundtripWorks<unsigned short>(std::numeric_limits<unsigned short>::max()));
  });
  ASSERT_THROW({
    createArgument<unsigned short>(QJsonValue(std::numeric_limits<unsigned short>::max() + 1ll));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned short>(QJsonValue(std::numeric_limits<unsigned short>::min() - 1ll));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned long long>(QJsonValue(5.5f));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned short>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned short>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned short>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, intArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<int>(0));
    ASSERT_TRUE(roundtripWorks<int>(5));
    ASSERT_TRUE(roundtripWorks<int>(std::numeric_limits<int>::lowest()));
    ASSERT_TRUE(roundtripWorks<int>(std::numeric_limits<int>::min()));
    ASSERT_TRUE(roundtripWorks<int>(std::numeric_limits<int>::max()));
  });
  ASSERT_THROW({
    createArgument<int>(QJsonValue(std::numeric_limits<int>::max() + 1ll));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<int>(QJsonValue(std::numeric_limits<int>::min() - 1ll));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned long long>(QJsonValue(5.5f));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<int>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<int>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<int>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, unsignedIntArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<unsigned int>(0));
    ASSERT_TRUE(roundtripWorks<unsigned int>(5));
    ASSERT_TRUE(roundtripWorks<unsigned int>(std::numeric_limits<unsigned int>::lowest()));
    ASSERT_TRUE(roundtripWorks<unsigned int>(std::numeric_limits<unsigned int>::min()));
    ASSERT_TRUE(roundtripWorks<unsigned int>(std::numeric_limits<unsigned int>::max()));
  });
  ASSERT_THROW({
    createArgument<unsigned int>(QJsonValue(std::numeric_limits<unsigned int>::max() + 1ll));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned int>(QJsonValue(std::numeric_limits<unsigned int>::lowest() - 1ll));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned long long>(QJsonValue(5.5f));
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

TEST(argumentTests, longArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<long>(0));
    ASSERT_TRUE(roundtripWorks<long>(5));
    ASSERT_TRUE(roundtripWorks<long>(std::numeric_limits<long>::lowest()));
    ASSERT_TRUE(roundtripWorks<long>(std::numeric_limits<long>::min()));
    ASSERT_TRUE(roundtripWorks<long>(std::numeric_limits<long>::max()));
  });
  ASSERT_THROW({
    createArgument<unsigned long long>(QJsonValue(5.5f));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<long>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<long>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<long>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, unsignedLongArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<unsigned long>(0));
    ASSERT_TRUE(roundtripWorks<unsigned long>(5));
    ASSERT_TRUE(roundtripWorks<unsigned long>(std::numeric_limits<unsigned long>::lowest()));
    ASSERT_TRUE(roundtripWorks<unsigned long>(std::numeric_limits<unsigned long>::min()));
    ASSERT_TRUE(roundtripWorks<unsigned long>(std::numeric_limits<unsigned long>::max()));
    //TODO Add support for full size unsigned longs with strings
  });
  ASSERT_THROW({
    createArgument<unsigned long long>(QJsonValue(5.5f));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned long>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned long>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned long>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, longLongArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<long long>(0));
    ASSERT_TRUE(roundtripWorks<long long>(5));
    ASSERT_TRUE(roundtripWorks<long long>(std::numeric_limits<long long>::lowest()));
    ASSERT_TRUE(roundtripWorks<long long>(std::numeric_limits<long long>::min()));
    ASSERT_TRUE(roundtripWorks<long long>(std::numeric_limits<long long>::max()));
    //TODO Add support for full size long longs with strings
  });
  ASSERT_THROW({
    createArgument<unsigned long long>(QJsonValue(5.5f));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<long long>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<long long>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<long long>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, unsignedLongLongArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<unsigned long long>(0));
    ASSERT_TRUE(roundtripWorks<unsigned long long>(5));
    ASSERT_TRUE(roundtripWorks<unsigned long long>(std::numeric_limits<unsigned long long>::lowest()));
    ASSERT_TRUE(roundtripWorks<unsigned long long>(std::numeric_limits<unsigned long long>::min()));
    ASSERT_TRUE(roundtripWorks<unsigned long long>(std::numeric_limits<unsigned long long>::max()));
    //TODO Add support for full size unsigned long longs with strings
  });
  ASSERT_THROW({
    createArgument<unsigned long long>(QJsonValue(5.5f));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned long long>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned long long>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned long long>(QJsonValue::Undefined);
  }, exceptionType);
}

//Boolean type

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

//Character types

TEST(argumentTests, charArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_EQ(getArgumentValue<char>(createArgument<char>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<char>(createArgument<char>(QJsonValue(127))), 127);
    ASSERT_EQ(getArgumentValue<unsigned char>(createArgument<char>(QJsonValue(128))), (unsigned char)128);
    ASSERT_EQ(getArgumentValue<unsigned char>(createArgument<char>(QJsonValue(-128))), (unsigned char)128);
    ASSERT_EQ(getArgumentValue<char>(createArgument<char>(QJsonValue(std::numeric_limits<char>::min()))), std::numeric_limits<char>::min());
    ASSERT_EQ(getArgumentValue<char>(createArgument<char>(QJsonValue(std::numeric_limits<char>::max()))), std::numeric_limits<char>::max());
    ASSERT_TRUE(roundtripWorks<char>(0));
    ASSERT_TRUE(roundtripWorks<char>(255));
    ASSERT_TRUE(roundtripWorks<char>(127));
  });
  ASSERT_THROW({
    createArgument<char>(QJsonValue(std::numeric_limits<signed char>::max() + 1i));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<char>(QJsonValue(std::numeric_limits<unsigned char>::min() - 1i));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<char>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<char>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<char>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, unsignedCharArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_EQ(getArgumentValue<unsigned char>(createArgument<unsigned char>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<unsigned char>(createArgument<unsigned char>(QJsonValue(127))), 127);
    ASSERT_EQ(getArgumentValue<unsigned char>(createArgument<unsigned char>(QJsonValue(std::numeric_limits<unsigned char>::min()))), std::numeric_limits<unsigned char>::min());
    ASSERT_EQ(getArgumentValue<unsigned char>(createArgument<unsigned char>(QJsonValue(std::numeric_limits<unsigned char>::max()))), std::numeric_limits<unsigned char>::max());
    ASSERT_TRUE(roundtripWorks<unsigned char>(0));
    ASSERT_TRUE(roundtripWorks<unsigned char>(255));
    ASSERT_TRUE(roundtripWorks<unsigned char>(127));
  });
  ASSERT_THROW({
    createArgument<unsigned char>(QJsonValue(std::numeric_limits<unsigned char>::max() + 1i));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned char>(QJsonValue(std::numeric_limits<unsigned char>::min() - 1i));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned char>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned char>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned char>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, signedCharArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_EQ(getArgumentValue<signed char>(createArgument<signed char>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<signed char>(createArgument<signed char>(QJsonValue(127))), 127);
    ASSERT_EQ(getArgumentValue<signed char>(createArgument<signed char>(QJsonValue(std::numeric_limits<signed char>::min()))), std::numeric_limits<signed char>::min());
    ASSERT_EQ(getArgumentValue<signed char>(createArgument<signed char>(QJsonValue(std::numeric_limits<signed char>::max()))), std::numeric_limits<signed char>::max());
    ASSERT_TRUE(roundtripWorks<signed char>(0));
    ASSERT_TRUE(roundtripWorks<signed char>(255));
    ASSERT_TRUE(roundtripWorks<signed char>(127));
  });
  ASSERT_THROW({
    createArgument<signed char>(QJsonValue(std::numeric_limits<signed char>::max() + 1i));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<signed char>(QJsonValue(std::numeric_limits<signed char>::min() - 1i));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<signed char>(QJsonValue("15"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<signed char>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<signed char>(QJsonValue::Undefined);
  }, exceptionType);
}

//Floating point types

TEST(argumentTests, floatArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<float>(0));
    ASSERT_TRUE(roundtripWorks<float>(5));
    ASSERT_TRUE(roundtripWorks<float>(std::numeric_limits<float>::lowest()));
    ASSERT_TRUE(roundtripWorks<float>(std::numeric_limits<float>::min()));
    ASSERT_TRUE(roundtripWorks<float>(std::numeric_limits<float>::max()));
    ASSERT_TRUE(roundtripWorks<float>(std::numeric_limits<float>::infinity()));
    ASSERT_TRUE(roundtripWorks<float>(-std::numeric_limits<float>::infinity()));
    ASSERT_TRUE(roundtripWorks<float>(std::numeric_limits<float>::min()*2));
    ASSERT_TRUE(std::isnan(getArgumentValue<float>(createArgument<float>(QJsonValue(std::numeric_limits<float>::quiet_NaN())))));
    ASSERT_TRUE(std::isnan(getArgumentValue<float>(createArgument<float>(QJsonValue(std::numeric_limits<float>::signaling_NaN())))));
    ASSERT_NE(getArgumentValue<float>(createArgument<float>(QJsonValue(DBL_MIN*2))), DBL_MIN*2);
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

TEST(argumentTests, doubleArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<double>(0));
    ASSERT_TRUE(roundtripWorks<double>(5));
    ASSERT_TRUE(roundtripWorks<double>(std::numeric_limits<double>::lowest()));
    ASSERT_TRUE(roundtripWorks<double>(std::numeric_limits<double>::min()));
    ASSERT_TRUE(roundtripWorks<double>(std::numeric_limits<double>::max()));
    ASSERT_TRUE(roundtripWorks<double>(std::numeric_limits<double>::infinity()));
    ASSERT_TRUE(roundtripWorks<double>(-std::numeric_limits<double>::infinity()));
    ASSERT_TRUE(roundtripWorks<double>(std::numeric_limits<double>::min()*2));
    ASSERT_TRUE(std::isnan(getArgumentValue<double>(createArgument<double>(QJsonValue(std::numeric_limits<double>::quiet_NaN())))));
    ASSERT_TRUE(std::isnan(getArgumentValue<double>(createArgument<double>(QJsonValue(std::numeric_limits<double>::signaling_NaN())))));
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

//Qt core types

TEST(argumentTests, QCharArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<QChar>('a'));
    ASSERT_TRUE(roundtripWorks<QChar>(L'ꨑ'));
    ASSERT_TRUE(roundtripWorks<QChar>(0));
    ASSERT_TRUE(roundtripWorks<QChar>(L'Δ'));
    ASSERT_TRUE(roundtripWorks<QChar>(QChar(QChar::LastValidCodePoint)));
  });
  ASSERT_THROW({
    createArgument<QChar>(QJsonValue(pow(2,16)));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<QChar>(QJsonValue(-1));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<QChar>(QJsonValue(""));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<QChar>(QJsonValue("aa"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<QChar>(QJsonValue("𐄂"));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<QChar>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<QChar>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, QStringArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<QString>(""));
    ASSERT_TRUE(roundtripWorks<QString>("hallo"));
    ASSERT_TRUE(roundtripWorks<QString>("test ꨑc"));
    QString stringWithNull("aaaaa");
    stringWithNull.insert(2, '\0');
    ASSERT_EQ(stringWithNull.size(), 6);
    ASSERT_TRUE(roundtripWorks<QString>(stringWithNull));
  });
  ASSERT_THROW({
    createArgument<QString>(18);
  }, exceptionType);
  ASSERT_THROW({
    createArgument<QString>('t');
  }, exceptionType);
  ASSERT_THROW({
    createArgument<QString>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<QString>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, QByteArrayArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_TRUE(roundtripWorks<QByteArray>(QByteArray{}));
    QByteArray array = QByteArrayLiteral("\x12\x00\xa4\x00\x00\x99");
    ASSERT_TRUE(roundtripWorks<QByteArray>(array));
    ASSERT_TRUE(roundtripWorks<QByteArray>("test ꨑ"));
  });
  ASSERT_THROW({
    createArgument<QByteArray>(18);
  }, exceptionType);
  ASSERT_THROW({
    createArgument<QByteArray>('t');
  }, exceptionType);
  ASSERT_THROW({
    createArgument<QByteArray>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<QByteArray>(QJsonValue::Undefined);
  }, exceptionType);
}

TEST(argumentTests, NullPtrTArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_EQ(getArgumentValue<std::nullptr_t>(createArgument<std::nullptr_t>(QJsonValue::Null)), nullptr);
    ASSERT_EQ(getArgumentValue<std::nullptr_t>(createArgument<std::nullptr_t>(0)), nullptr);
  });
  ASSERT_THROW({
    createArgument<std::nullptr_t>('t');
  }, exceptionType);
  ASSERT_THROW({
    createArgument<std::nullptr_t>(QJsonValue(false));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<std::nullptr_t>(QJsonValue::Undefined);
  }, exceptionType);
}




#endif
