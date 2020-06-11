#include <QCoreApplication>

#include <QList>
#include <QString>
#include "test/testservice.h"
#include "src/jsonrpcserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    JsonRpcServer<TestService, true>* test = new JsonRpcServer<TestService>(56730);
    test->startListening();

    return a.exec();
}
