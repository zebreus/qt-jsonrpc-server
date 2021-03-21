INCLUDEPATH *= $$PWD/include
DEPENDPATH *= $$PWD

QT += websockets

CONFIG += c++2a

SOURCES *= \
    $$PWD/src/argument.cpp \
    $$PWD/src/call.cpp \
    $$PWD/src/callmanager.cpp \
    $$PWD/src/slotconverter.cpp \
    $$PWD/src/connection.cpp \
    $$PWD/src/error.cpp \
    $$PWD/src/interfacedescription.cpp \
    $$PWD/src/message.cpp \
    $$PWD/src/messageprocessor.cpp \
    $$PWD/src/methoddescription.cpp \
    $$PWD/src/request.cpp \
    $$PWD/src/response.cpp \
    $$PWD/src/exceptions.cpp \
    $$PWD/src/signalconverter.cpp

HEADERS *= \
    $$PWD/include/argument.h \
    $$PWD/include/call.h \
    $$PWD/include/callmanager.h \
    $$PWD/include/connection.h \
    $$PWD/include/error.h \
    $$PWD/include/message.h \
    $$PWD/include/messageprocessor.h \
    $$PWD/include/request.h \
    $$PWD/include/response.h \
    $$PWD/include/server.h \
    $$PWD/include/exceptions.h \
    $$PWD/include/signalconverter.h \
    $$PWD/include/interfacedescription.h \
    $$PWD/include/methoddescription.h \
    $$PWD/include/slotconverter.h
    
test{
    LIBS *= -lgtest
    CONFIG *= thread
    QT *= testlib
    
    HEADERS += $$PWD/tests/mocktarget.hpp
    SOURCES += $$PWD/tests/argumenttest.cpp \
               $$PWD/tests/calltest.cpp \
               $$PWD/tests/exceptionstest.cpp \
               $$PWD/tests/callmanagertest.cpp \
               $$PWD/tests/signalconvertertest.cpp \
               $$PWD/tests/slotconvertertest.cpp \
               $$PWD/tests/servertest.cpp \
               $$PWD/tests/mocktarget.cpp \
               $$PWD/tests/connectiontest.cpp \
               $$PWD/tests/interfacedescriptiontest.cpp \
               $$PWD/tests/methoddescriptiontest.cpp \
               $$PWD/tests/main.cpp
}
