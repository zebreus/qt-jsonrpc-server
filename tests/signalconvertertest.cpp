#ifndef SIGNAL_CONVERTER_TEST_CPP
#define SIGNAL_CONVERTER_TEST_CPP

#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include <QJsonValue>
#include <signalconverter.h>
#include <QCoreApplication>
#include <QEventLoop>
#include <QObject>
#include "calltest.hpp"

using namespace testing;
using namespace jsonrpc;

TEST(signalConverterTests, testSignalsGetConverted) {
    MockTarget emitter;
    SignalConverter converter;
    converter.attach(&emitter);

    QString returnValue;
    QObject::connect(&converter, &SignalConverter::convertedSignal, [&returnValue](QString text){
        returnValue = text;
    });

    emit emitter.signalA();

    QTime limit = QTime::currentTime().addMSecs(500);
    while (QTime::currentTime() < limit && returnValue == "") {
      QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }

    ASSERT_EQ(returnValue, "signalA");
}

TEST(signalConverterTests, testMethodsAndSlotsDoNotGetConverted) {
    MockTarget emitter;
    SignalConverter converter;
    converter.attach(&emitter);

    bool convertedSignal = false;
    QObject::connect(&converter, &SignalConverter::convertedSignal, [&convertedSignal](QString){
        convertedSignal = true;
    });

    //emit is used for clarity
    //Slot
    emit emitter.noParams();
    //Method
    emit emitter.emptyMethod();

    QTime limit = QTime::currentTime().addMSecs(200);
    while (QTime::currentTime() < limit) {
      QCoreApplication::processEvents(QEventLoop::AllEvents, 50);
    }

    ASSERT_FALSE(convertedSignal);
}

#endif
