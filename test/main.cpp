#include <server.h>

#include <QCoreApplication>
#include <QList>
#include <QString>

#include "test/testservice.h"

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);

  jsonrpc::Server<TestService> test(56730);
  test.setConstructorArguments(42, "test");
  test.startListening();

  return a.exec();
}
