QT -= gui
QT += websockets

CONFIG += c++17 console
CONFIG -= app_bundle

INCLUDEPATH += include/

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/argument.cpp \
    src/call.cpp \
    src/callmanager.cpp \
    src/connection.cpp \
    src/error.cpp \
    src/message.cpp \
    src/messageprocessor.cpp \
    src/request.cpp \
    src/requestmanager.cpp \
    src/response.cpp \
    src/exceptions.cpp \
    test/testservice.cpp \
    test/main.cpp

HEADERS += \
    include/argument.h \
    include/call.h \
    include/callmanager.h \
    include/connection.h \
    include/error.h \
    include/message.h \
    include/messageprocessor.h \
    include/request.h \
    include/requestmanager.h \
    include/response.h \
    include/server.h \
    include/exceptions.h \
    test/testservice.h

test{
    include($$PWD/libs/gtest/gtest_dependency.pri)

    QT += testlib
    TEMPLATE = app
    TARGET = qt-jsonrpc-tests
    INCLUDEPATH += src

    CONFIG += thread
    LIBS += -lgtest -lgtest_main

    SOURCES -= test/testservice.cpp
    SOURCES -= test/main.cpp
    HEADERS -= test/testservice.h
    HEADERS += tests/calltest.hpp
    SOURCES += tests/argumenttest.cpp \
               tests/calltest.cpp
}
else{
    message(Building app)
    TEMPLATE = app
    TARGET = qt-jsonrpc
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
