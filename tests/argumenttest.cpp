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

//Integer types

TEST(argumentTests, shortArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_EQ(getArgumentValue<short>(createArgument<short>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<short>(createArgument<short>(QJsonValue(5))), 5);
    ASSERT_EQ(getArgumentValue<short>(createArgument<short>(QJsonValue(std::numeric_limits<short>::min()))), std::numeric_limits<short>::min());
    ASSERT_EQ(getArgumentValue<short>(createArgument<short>(QJsonValue(std::numeric_limits<short>::max()))), std::numeric_limits<short>::max());
  });
  ASSERT_THROW({
    createArgument<short>(QJsonValue(std::numeric_limits<short>::max() + 1ll));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<short>(QJsonValue(std::numeric_limits<short>::min() - 1ll));
  }, exceptionType);
  ASSERT_THROW({
    createArgument<unsigned long long>(QJsonValue(5.5f));
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
    ASSERT_EQ(getArgumentValue<unsigned short>(createArgument<unsigned short>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<unsigned short>(createArgument<unsigned short>(QJsonValue(std::numeric_limits<unsigned short>::lowest()))), std::numeric_limits<unsigned short>::lowest());
    ASSERT_EQ(getArgumentValue<unsigned short>(createArgument<unsigned short>(QJsonValue(std::numeric_limits<unsigned short>::min()))), std::numeric_limits<unsigned short>::min());
    ASSERT_EQ(getArgumentValue<unsigned short>(createArgument<unsigned short>(QJsonValue(std::numeric_limits<unsigned short>::max()))), std::numeric_limits<unsigned short>::max());
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
    ASSERT_EQ(getArgumentValue<int>(createArgument<int>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<int>(createArgument<int>(QJsonValue(std::numeric_limits<int>::lowest()))), std::numeric_limits<int>::lowest());
    ASSERT_EQ(getArgumentValue<int>(createArgument<int>(QJsonValue(std::numeric_limits<int>::min()))), std::numeric_limits<int>::min());
    ASSERT_EQ(getArgumentValue<int>(createArgument<int>(QJsonValue(std::numeric_limits<int>::max()))), std::numeric_limits<int>::max());
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
    ASSERT_EQ(getArgumentValue<unsigned int>(createArgument<unsigned int>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<unsigned int>(createArgument<unsigned int>(QJsonValue((double)std::numeric_limits<unsigned int>::lowest()))), std::numeric_limits<unsigned int>::lowest());
    ASSERT_EQ(getArgumentValue<unsigned int>(createArgument<unsigned int>(QJsonValue((double)std::numeric_limits<unsigned int>::min()))), std::numeric_limits<unsigned int>::min());
    ASSERT_EQ(getArgumentValue<unsigned int>(createArgument<unsigned int>(QJsonValue((double)std::numeric_limits<unsigned int>::max()))), std::numeric_limits<unsigned int>::max());
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
    ASSERT_EQ(getArgumentValue<long>(createArgument<long>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<long>(createArgument<long>(QJsonValue(std::numeric_limits<int>::lowest()))), std::numeric_limits<int>::lowest());
    ASSERT_EQ(getArgumentValue<long>(createArgument<long>(QJsonValue(std::numeric_limits<int>::min()))), std::numeric_limits<int>::min());
    ASSERT_EQ(getArgumentValue<long>(createArgument<long>(QJsonValue(std::numeric_limits<int>::max()))), std::numeric_limits<int>::max());
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
    ASSERT_EQ(getArgumentValue<unsigned long>(createArgument<unsigned long>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<unsigned long>(createArgument<unsigned long>(QJsonValue(std::numeric_limits<int>::max()))), std::numeric_limits<int>::max());
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
    ASSERT_EQ(getArgumentValue<long long>(createArgument<long long>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<long long>(createArgument<long long>(QJsonValue(std::numeric_limits<int>::lowest()))), std::numeric_limits<int>::lowest());
    ASSERT_EQ(getArgumentValue<long long>(createArgument<long long>(QJsonValue(std::numeric_limits<int>::max()))), std::numeric_limits<int>::max());
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
    ASSERT_EQ(getArgumentValue<unsigned long long>(createArgument<unsigned long long>(QJsonValue(0))), 0);
    ASSERT_EQ(getArgumentValue<unsigned long long>(createArgument<unsigned long long>(QJsonValue(std::numeric_limits<int>::max()))), std::numeric_limits<int>::max());
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

//Qt core types

TEST(argumentTests, QCharArgumentTest) {
  ASSERT_NO_THROW({
    ASSERT_EQ(getArgumentValue<QChar>(createArgument<QChar>(QJsonValue('a'))), QChar('a'));
    ASSERT_EQ(getArgumentValue<QChar>(createArgument<QChar>(QJsonValue(97))), QChar('a'));
    ASSERT_EQ(getArgumentValue<QChar>(createArgument<QChar>(QJsonValue(L'ꨑ'))), L'ꨑ');
    ASSERT_EQ(getArgumentValue<QChar>(createArgument<QChar>(QJsonValue(0xAA11))), L'ꨑ');
    ASSERT_EQ(getArgumentValue<QChar>(createArgument<QChar>(QJsonValue("Δ"))), L'Δ');
    ASSERT_EQ(getArgumentValue<QChar>(createArgument<QChar>(QJsonValue('\0'))), L'\0');
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
    ASSERT_EQ(getArgumentValue<QString>(createArgument<QString>(QJsonValue(""))), "");
    ASSERT_EQ(getArgumentValue<QString>(createArgument<QString>(QJsonValue("hallo"))), "hallo");
    ASSERT_EQ(getArgumentValue<QString>(createArgument<QString>(QJsonValue("test ꨑ"))), "test ꨑ");
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
    ASSERT_EQ(getArgumentValue<QByteArray>(createArgument<QByteArray>(QJsonValue(""))), "");
    ASSERT_EQ(getArgumentValue<QByteArray>(createArgument<QByteArray>(QJsonValue("hallo"))), "hallo");
    ASSERT_EQ(getArgumentValue<QByteArray>(createArgument<QByteArray>(QJsonValue("test ꨑ"))), "test ꨑ");
    ASSERT_EQ(getArgumentValue<QByteArray>(createArgument<QByteArray>(QJsonValue("tes\0t ꨑ"))), QByteArray("tes\0t ꨑ"));
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
  });
  ASSERT_THROW({
    createArgument<std::nullptr_t>(0);
  }, exceptionType);
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
