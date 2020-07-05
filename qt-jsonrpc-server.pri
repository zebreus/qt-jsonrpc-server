INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD

QT += websockets

SOURCES += \
    $$PWD/src/argument.cpp \
    $$PWD/src/boolargument.cpp \
    $$PWD/src/call.cpp \
    $$PWD/src/callmanager.cpp \
    $$PWD/src/connection.cpp \
    $$PWD/src/error.cpp \
    $$PWD/src/intargument.cpp \
    $$PWD/src/message.cpp \
    $$PWD/src/messageprocessor.cpp \
    $$PWD/src/request.cpp \
    $$PWD/src/requestmanager.cpp \
    $$PWD/src/response.cpp

HEADERS += \
    $$PWD/include/argument.h \
    $$PWD/include/boolargument.h \
    $$PWD/include/call.h \
    $$PWD/include/callmanager.h \
    $$PWD/include/connection.h \
    $$PWD/include/error.h \
    $$PWD/include/intargument.h \
    $$PWD/include/message.h \
    $$PWD/include/messageprocessor.h \
    $$PWD/include/request.h \
    $$PWD/include/requestmanager.h \
    $$PWD/include/response.h \
    $$PWD/include/server.h
