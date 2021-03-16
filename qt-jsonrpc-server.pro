QT -= gui
QT += websockets

CONFIG += c++2a console
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
    src/slotconverter.cpp \
    src/connection.cpp \
    src/error.cpp \
    src/interfacedescription.cpp \
    src/message.cpp \
    src/messageprocessor.cpp \
    src/methoddescription.cpp \
    src/request.cpp \
    src/response.cpp \
    src/exceptions.cpp \
    src/signalconverter.cpp

HEADERS += \
    include/argument.h \
    include/call.h \
    include/callmanager.h \
    include/connection.h \
    include/error.h \
    include/message.h \
    include/messageprocessor.h \
    include/request.h \
    include/response.h \
    include/server.h \
    include/exceptions.h \
    include/signalconverter.h \
    include/interfacedescription.h \
    include/methoddescription.h \
    include/slotconverter.h

test{
    include($$PWD/libs/gtest/gtest_dependency.pri)

    QT += testlib
    TEMPLATE = app
    TARGET = qt-jsonrpc-tests
    INCLUDEPATH += src

    CONFIG += thread
    LIBS += -lgtest

    HEADERS += tests/mocktarget.hpp
    SOURCES += tests/argumenttest.cpp \
               tests/calltest.cpp \
               tests/exceptionstest.cpp \
               tests/callmanagertest.cpp \
               tests/signalconvertertest.cpp \
               tests/slotconvertertest.cpp \
               tests/servertest.cpp \
               tests/mocktarget.cpp \
               tests/connectiontest.cpp \
               tests/interfacedescriptiontest.cpp \
               tests/methoddescriptiontest.cpp \
               tests/main.cpp
} else:demo{
    message(Building demo)
    TEMPLATE = app
    TARGET = qt-jsonrpc
    SOURCES += demo/main.cpp \
               tests/mocktarget.cpp
    HEADERS += tests/mocktarget.hpp
}else{
    message(Building demo)
    TEMPLATE = lib
    TARGET = qt-jsonrpc
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
