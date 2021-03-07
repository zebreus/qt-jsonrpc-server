#ifndef CALL_TEST_HPP
#define CALL_TEST_HPP

#include <QObject>
#include <QString>

class SuperMock : public QObject{
    Q_OBJECT
public slots:
    inline int superSlot(){
        return 564;
    };
};

//TODO Find out if googlemock can help with this
class MockTarget : public SuperMock{
    Q_OBJECT
public:
    inline int emptyMethod(){
        return 345;
    };
public slots:
    inline int addNumbers(int a, int b){
        return a+b;
    };
    inline QString echoString(const QString& input){
        return input;
    };
    inline int noParams(){
        return 564;
    };
    inline void emitSignalA(){
        emit signalA();
    };
    inline void emitSignalB(){
        emit signalB("text");
    };
private slots:
    inline int privateSlot(){
        return 564;
    };
signals:
    void signalA();
    void signalB(QString text);
};


#endif
