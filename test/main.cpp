#include <QCoreApplication>

#include <QList>
#include <QString>
#include "test/testservice.h"
#include <server.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    jsonrpc::Server<TestService> test(56730);
    test.startListening();

    return a.exec();
}
