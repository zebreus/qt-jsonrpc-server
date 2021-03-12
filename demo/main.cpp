#include <server.h>

#include <QCoreApplication>
#include <QList>
#include <QString>

#include "tests/mocktarget.hpp"

int main(int argc, char* argv[]) {
  QCoreApplication a(argc, argv);

  jsonrpc::Server<MockTarget> test(56730);
  test.setConstructorArguments(42);
  test.startListening();

  return a.exec();
}
