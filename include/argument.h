#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <QMetaType>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDate>
#include <cmath>


//TODO Make argument templated instead of having many subclasses
class Argument {

public:
    QGenericArgument getArgument();
    virtual ~Argument();

protected:
    Argument();

private:
    Argument(Argument const&) = delete;
    Argument& operator=(Argument const&) = delete;
protected:
    QGenericArgument argument;
};

template<typename T>
class ArgumentImplementation : public Argument{
public:
    ArgumentImplementation(const QJsonValue& argument);
private:
    void setValue(const T& t);
    QSharedPointer<T> value;
};




#endif // ARGUMENT_H
