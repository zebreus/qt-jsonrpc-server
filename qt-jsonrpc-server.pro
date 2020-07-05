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
    src/doubleargument.cpp \
    src/qcharargument.cpp \
    src/qjsonvalueargument.cpp \
    src/qstringargument.cpp \
    src/uintargument.cpp \
    src/argument.cpp \
    src/boolargument.cpp \
    src/call.cpp \
    src/callmanager.cpp \
    src/connection.cpp \
    src/error.cpp \
    src/intargument.cpp \
    src/message.cpp \
    src/messageprocessor.cpp \
    src/request.cpp \
    src/requestmanager.cpp \
    src/response.cpp \
    test/testservice.cpp \
    test/main.cpp

HEADERS += \
    include/argument.h \
    include/boolargument.h \
    include/call.h \
    include/callmanager.h \
    include/connection.h \
    include/doubleargument.h \
    include/error.h \
    include/intargument.h \
    include/message.h \
    include/messageprocessor.h \
    include/qcharargument.h \
    include/qjsonvalueargument.h \
    include/qstringargument.h \
    include/request.h \
    include/requestmanager.h \
    include/response.h \
    include/server.h \
    include/uintargument.h \
    test/testservice.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
