#ifndef EXCEPTIONS_TEST_CPP
#define EXCEPTIONS_TEST_CPP

#include <exceptions.h>
#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>

#include <QJsonValue>

using namespace testing;
using namespace jsonrpc;

TEST(exceptionsTests, invalidSignatureInheritsFromJsonrpcException) {
  ASSERT_THROW({ throw exceptions::InvalidSignature("message"); }, exceptions::JsonrpcException);
}

TEST(exceptionsTests, invokationFailedInheritsFromJsonrpcException) {
  ASSERT_THROW({ throw exceptions::InvokationFailed("message"); }, exceptions::JsonrpcException);
}

TEST(exceptionsTests, unknownMethodNameInheritsFromInvalidSignature) {
  ASSERT_THROW({ throw exceptions::UnknownMethodName("methodName"); }, exceptions::InvalidSignature);
}

TEST(exceptionsTests, unknownMethodnameSetsMessage) {
  QString methodName = "methodName";
  exceptions::UnknownMethodName exception(methodName);
  ASSERT_NE(exception.getMessage(), "");
  ASSERT_NE(exception.getMessage(), methodName);
}

TEST(exceptionsTests, wrongArgumentCountInheritsFromInvalidSignature) {
  ASSERT_THROW({ throw exceptions::WrongArgumentCount(1, 1); }, exceptions::InvalidSignature);
}

TEST(exceptionsTests, WrongArgumentTypeInheritsFromInvalidSignature) {
  ASSERT_THROW({ throw exceptions::WrongArgumentType("a", QString("b")); }, exceptions::InvalidSignature);
}

#endif
