#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <QMetaType>
#include <QJsonValue>


//TODO Make argument templated instead of having many subclasses
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
