#ifndef TESTSERVICE_H
#define TESTSERVICE_H

#include <QObject>
#include <QDebug>

using namespace std;

class TestService : public QObject
{
    Q_OBJECT
public:
    explicit TestService(QObject *parent = nullptr);
    explicit TestService(int number, QString string, QObject *parent = nullptr);

public Q_SLOTS:
    QString echoTest(QString value);
    int additionTest(int a, int b);
    int subtractionTest(int a, int b);


signals:

};

#endif // TESTSERVICE_H
