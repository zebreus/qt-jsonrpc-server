#include "testservice.h"

TestService::TestService(QObject *parent) : QObject(parent)
{

}

QString TestService::echoTest(QString value)
{
    qDebug() << "echoTest called with " << value ;
    return value;
}

int TestService::additionTest(int a, int b)
{
    qDebug() << "additionTest called with " << a << "+" << b << " -> " << a+b;
    return a+b;
}
