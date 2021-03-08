#ifndef MOCK_TARGET_HPP
#define MOCK_TARGET_HPP

#include <QObject>
#include <QString>

class SuperMock: public QObject {
  Q_OBJECT
 public:
  explicit SuperMock(QObject* parent = nullptr);
  virtual ~SuperMock();
 public slots:
  int superSlot();
};

// TODO Find out if googlemock can help with this
class MockTarget: public SuperMock {
  Q_OBJECT
 public:
  explicit MockTarget(QObject* parent = nullptr);
  explicit MockTarget(int number, QObject* parent = nullptr);
  virtual ~MockTarget();
  int emptyMethod();
 public slots:
  int addNumbers(int a, int b);
  QString echoString(const QString& input);
  int noParams();
  void emitSignalA();
  void emitSignalB();
 private slots:
  int privateSlot();
 signals:
  void signalA();
  void signalB(QString text);

 private:
  int constructorNumber;
};

#endif
