#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <QMetaType>
#include <QJsonValue>

class Argument {

 public:

    virtual QGenericArgument getArgument();

    virtual ~Argument();

 protected:

    Argument();

  private:
    Argument(Argument const&) = delete;
    Argument& operator=(Argument const&) = delete;
 protected:
    QGenericArgument argument;
};
#endif // ARGUMENT_H
