#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <QMetaType>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDate>
#include <cmath>
#include <QSize>

#if defined(QT_GUI_LIB) or defined(QT_MODULE_GUI)
#include <QCursor>
#include <QPolygon>
#include <QColor>
#include <QColorSpace>
#include <QLine>
#include <QTextLength>
#include <QIcon>
#include <QPalette>
#include <QBitArray>
#include <QtGui>
#endif
#if defined(QT_WIDGETS_LIB) or defined(QT_MODULE_WIDGETS)
#include <QSizePolicy>
#endif

class Argument {

public:
    QGenericArgument getArgument();
    virtual ~Argument();

    static Argument* create(const int requiredTypeId, const QJsonValue& value = QJsonValue::Undefined);
protected:
    template<typename T>
    static Argument* createArgument(const QJsonValue& jsonValue);
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
    ~ArgumentImplementation() override;
    ArgumentImplementation(const QJsonValue& argument);
    ArgumentImplementation();
private:
    void setValue(const T& t);
    T* value;
};




#endif // ARGUMENT_H
