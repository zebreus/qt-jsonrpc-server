#include "argument.h"

QGenericArgument Argument::getArgument()
{
    return argument;
}

QJsonValue Argument::getJson()
{
    return "t";
}

Argument::~Argument()
{

}

Argument::Argument()
{

}

template<typename T>
void ArgumentImplementation<T>::setValue(const T& value){
    this->value = new T(value);
    argument = Q_ARG(T, *this->value);
}

template<typename T>
ArgumentImplementation<T>::ArgumentImplementation(const QJsonValue&){
    static_assert(!std::is_same<T,T>::value,"Argument is not implemented for the type");
}

template<>
ArgumentImplementation<bool>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isBool() ){
        setValue(argument.toBool());
    }else{
        throw QString("Parameter of type %1 cannot be converted to boolean").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<bool>::getJson(){
    return QJsonValue(*value);
}

template<>
ArgumentImplementation<int>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw QString("Cannot convert number to int, because it is not integer");
        }
        if (integerPart > (double)INT_MAX || integerPart < (double)INT_MIN){
            throw QString("Cannot convert number to int, because it is out of range");
        }
        setValue((int)integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<int>::getJson(){
    return QJsonValue(*value);
}

template<>
ArgumentImplementation<unsigned int>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw QString("Cannot convert number to unsigned int, because it is not integer");
        }
        if (integerPart > (double)UINT_MAX || integerPart < (double)0){
            throw QString("Cannot convert number to unsigned int, because it is out of range");
        }
        setValue((unsigned int)integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<unsigned int>::getJson(){
    return QJsonValue((long long)*value);
}

template<>
ArgumentImplementation<double>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        setValue(argument.toDouble());
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<double>::getJson(){
    return QJsonValue(*value);
}

//TODO char from int
template<>
ArgumentImplementation<QChar>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isString() ){
        QString stringValue = argument.toString();
        if(stringValue.length() != 1){
            throw QString("Cannot convert string to char, because it is not one char long");
        }
        setValue(stringValue.front());
    }else if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw QString("Cannot convert number to QChar, because it is not integer");
        }
        if (integerPart > (double)USHRT_MAX || integerPart < (double)0){
            throw QString("Cannot convert number to QChar, because it is out of range");
        }
        setValue((unsigned short)integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to text").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<QChar>::getJson(){
    //TODO maybe represent as short string
    return QJsonValue(value->digitValue());
}

template<>
ArgumentImplementation<QString>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isString() ){
        setValue(argument.toString());
    }else{
        throw QString("Parameter of type %1 cannot be converted to text").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<QString>::getJson(){
    return QJsonValue(*value);
}

template<>
ArgumentImplementation<QByteArray>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isString() ){
        //TODO Check if toUtf8 is appropriate
        setValue(argument.toString().toUtf8());
    }else if(argument.isArray()){
        const QJsonArray& array = argument.toArray();
        QScopedArrayPointer<char> charPointer(new char[array.size()]);
        int position = 0;
        for(const QJsonValue& val: array){
            if( val.isDouble() ){
                double doubleValue = val.toDouble();
                double integerPart;
                if(std::modf(doubleValue, &integerPart) != 0.0){
                    throw QString("Cannot convert number to byte for QByteArray, because it is not integer");
                }
                if (integerPart > (double)UCHAR_MAX || integerPart < (double)SCHAR_MIN){
                    throw QString("Cannot convert number to byte for QByteArray, because it is out of range");
                }
                charPointer[position] = (char)(int)integerPart;
            }else if( val.isString() ){
                QByteArray charArray = val.toString().toUtf8();
                if(charArray.length() != 1){
                    throw QString("Cannot convert string to byte for QByteArray, because it is longer than one byte");
                }
                charPointer[position] = charArray.at(0);
            }else{
                throw QString("Parameter of type %1 cannot be converted to byte for QByteArray").arg(QString(val.type()));
            }
        }
        setValue(QByteArray(charPointer.get(), array.size()));
    }else{
        throw QString("Parameter of type %1 cannot be converted to text").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<QByteArray>::getJson(){
    QJsonArray array;
    for(char c: *value){
        array.append(QJsonValue(c));
    }
    return array;
}

template<>
ArgumentImplementation<std::nullptr_t>::ArgumentImplementation(const QJsonValue& argument){
    //TODO nullptr_t is probably best represented by null in json, maybe also undefined idk
    if( argument.isNull() ){
        setValue(nullptr);
    }else if(argument.isDouble()){
        if(argument.toDouble() == 0){
            setValue(nullptr);
        }else{
            throw QString("Parameter of type %1 cannot be converted to nullptr").arg(QString(argument.type()));
        }
    }else{
        throw QString("Parameter of type %1 cannot be converted to nullptr").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<std::nullptr_t>::getJson(){
    return QJsonValue::Null;
}

template<>
ArgumentImplementation<void*>::ArgumentImplementation(const QJsonValue& argument){
    //voidpointer will always fails
    throw QString("Parameter of type %1 cannot be converted to void pointer").arg(QString(argument.type()));
}

template<>
QJsonValue ArgumentImplementation<void*>::getJson(){
    throw QJsonValue::Undefined;
}

template<>
ArgumentImplementation<long>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw QString("Cannot convert number to long, because it is not integer");
        }
        if (integerPart > (double)LONG_MAX || integerPart < (double)LONG_MIN){
            throw QString("Cannot convert number to long, because it is out of range");
        }
        setValue((long)integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<long>::getJson(){
    //TODO A long can be unrepresentable as a double, if it is too big. It should be put into a string in that case.
    return QJsonValue((double)*value);
}

template<>
ArgumentImplementation<long long>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw QString("Cannot convert number to long long, because it is not integer");
        }
        if (integerPart > (double)LLONG_MAX || integerPart < (double)LLONG_MIN){
            throw QString("Cannot convert number to long long, because it is out of range");
        }
        setValue((long long)integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<long long>::getJson(){
    //TODO A long long can be unrepresentable as a double, if it is too big. It should be put into a string in that case.
    return QJsonValue((double)*value);
}

template<>
ArgumentImplementation<short>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw QString("Cannot convert number to short, because it is not integer");
        }
        if (integerPart > (double)SHRT_MAX || integerPart < (double)SHRT_MIN){
            throw QString("Cannot convert number to short, because it is out of range");
        }
        setValue(integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<short>::getJson(){
    return QJsonValue(*value);
}

template<>
ArgumentImplementation<char>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw QString("Cannot convert number to char, because it is not integer");
        }
        if (integerPart > (double)UCHAR_MAX || integerPart < (double)SCHAR_MIN){
            throw QString("Cannot convert number to char, because it is out of range");
        }
        setValue((char)(int)integerPart);
    }else if( argument.isString() ){
        QByteArray charArray = argument.toString().toUtf8();
        if(charArray.length() != 1){
            throw QString("Cannot convert string to char, because it is not one char long");
        }
        setValue(charArray.at(0));
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<char>::getJson(){
    return QJsonValue(*value);
}

template<>
ArgumentImplementation<unsigned long>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw QString("Cannot convert number to unsigned long, because it is not integer");
        }
        if (integerPart > (double)ULONG_MAX || integerPart < (double)0){
            throw QString("Cannot convert number to unsigned long, because it is out of range");
        }
        setValue(integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<unsigned long>::getJson(){
    //TODO An unsigned long can be unrepresentable as a double, if it is too big. It should be put into a string in that case.
    return QJsonValue((double)*value);
}

template<>
ArgumentImplementation<unsigned long long>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw QString("Cannot convert number to unsigned long long, because it is not integer");
        }
        if (integerPart > (double)ULLONG_MAX || integerPart < (double)0){
            throw QString("Cannot convert number to unsigned long long, because it is out of range");
        }
        setValue(integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<unsigned long long>::getJson(){
    //TODO An unsigned long can be unrepresentable as a double, if it is too big. It should be put into a string in that case.
    return QJsonValue((double)*value);
}

template<>
ArgumentImplementation<unsigned short>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw QString("Cannot convert number to unsigned short, because it is not integer");
        }
        if (integerPart > (double)USHRT_MAX || integerPart < (double)0){
            throw QString("Cannot convert number to unsigned short, because it is out of range");
        }
        setValue(integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<unsigned short>::getJson(){
    return QJsonValue(*value);
}

template<>
ArgumentImplementation<signed char>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw QString("Cannot convert number to unsigned char, because it is not integer");
        }
        if (integerPart > (double)SCHAR_MAX || integerPart < (double)SCHAR_MIN){
            throw QString("Cannot convert number to unsigned char, because it is out of range");
        }
        setValue(integerPart);
    }else if( argument.isString() ){
        QByteArray charArray = argument.toString().toUtf8();
        if(charArray.length() != 1){
            throw QString("Cannot convert string to char, because it is not one char long");
        }
        setValue(charArray.at(0));
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<signed char>::getJson(){
    return QJsonValue(*value);
}

template<>
ArgumentImplementation<unsigned char>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw QString("Cannot convert number to unsigned char, because it is not integer");
        }
        if (integerPart > (double)UCHAR_MAX || integerPart < (double)0){
            throw QString("Cannot convert number to unsigned char, because it is out of range");
        }
        setValue(integerPart);
    }else if( argument.isString() ){
        QByteArray charArray = argument.toString().toUtf8();
        if(charArray.length() != 1){
            throw QString("Cannot convert string to char, because it is not one char long");
        }
        setValue(charArray.at(0));
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<unsigned char>::getJson(){
    return QJsonValue(*value);
}

template<>
ArgumentImplementation<float>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        //TODO double(32bit) -> float(64bit) -> loss of precision, maybe throw
        double doubleValue = argument.toDouble();
        setValue(doubleValue);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<float>::getJson(){
    return QJsonValue(*value);
}

template<>
ArgumentImplementation<QObject*>::ArgumentImplementation(const QJsonValue& argument){
    //pointers will always fails
    throw QString("Parameter of type %1 cannot be converted to QObject pointer").arg(QString(argument.type()));
}

template<>
QJsonValue ArgumentImplementation<QObject*>::getJson(){
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QVariant>::ArgumentImplementation(const QJsonValue& argument){
    //TODO maybe check variant content
    setValue(argument.toVariant());
}

template<>
QJsonValue ArgumentImplementation<QVariant>::getJson(){
    //TODO should be ok, as long as the variant only gets constructed from json value
    return QJsonValue::fromVariant(*value);
}

template<>
ArgumentImplementation<QCursor>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QCursor type
    throw QString("QCursor type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QCursor>::getJson(){
    //TODO implement QCursor type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QDate>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isString() ){
        //Try TextDate and ISODate formats
        QString argumentString = argument.toString();
        QDate date = QDate::fromString(argumentString, Qt::TextDate);
        if(date.isValid()){
            //Date construction successful
            setValue(date);
        }else{
            //try iso format
            date = QDate::fromString(argumentString, Qt::ISODate);
            if(date.isValid()){
                setValue(date);
            }else{
                throw QString("Cannot convert string to QDate, because it is formatted incorrectly. Use TextDate or ISODate.");
            }
        }
    }if( argument.isArray() ){
        QJsonArray jsonArray = argument.toArray();
        if(jsonArray.size() == 3){
            if(jsonArray.at(0).isDouble() && jsonArray.at(1).isDouble() && jsonArray.at(2).isDouble()){
                QDate date(jsonArray.at(0).toDouble(),jsonArray.at(1).toDouble(),jsonArray.at(2).toDouble());
                if(date.isValid()){
                    setValue(date);
                }else{
                    throw QString("Cannot convert array to QDate, because it's values are out of range.");
                }
            }else{
                throw QString("Cannot convert array to QDate, because it's values are not integers.");
            }
        }else{
            throw QString("Cannot convert array to QDate, because it has the wrong length.");
        }
    }else{
        throw QString("Parameter of type %1 cannot be converted to date. Required format \"ddd MMM d yyyy\"").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<QDate>::getJson(){
    return value->toString("yyyy-MM-dd");
}

template<>
ArgumentImplementation<QSize>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QSize type
    throw QString("QSize type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QSize>::getJson(){
    //TODO implement QSize type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QTime>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QTime type
    throw QString("QTime type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QTime>::getJson(){
    return value->toString("HH:mm:ss");
}

template<>
ArgumentImplementation<QVariantList>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVariantList type
    throw QString("QVariantList type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QVariantList>::getJson(){
    //TODO implement QVariantList type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QPolygon>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPolygon type
    throw QString("QPolygon type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QPolygon>::getJson(){
    //TODO implement QPolygon type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QPolygonF>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPolygonF type
    throw QString("QPolygonF type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QPolygonF>::getJson(){
    //TODO implement QPolygonF type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QColor>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QColor type
    throw QString("QColor type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QColor>::getJson(){
    //TODO implement QColor type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QColorSpace>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QColorSpace type
    throw QString("QColorSpace type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QColorSpace>::getJson(){
    //TODO implement QColorSpace type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QSizeF>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QSizeF type
    throw QString("QSizeF type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QSizeF>::getJson(){
    //TODO implement QSizeF type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QRectF>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QRectF type
    throw QString("QRectF type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QRectF>::getJson(){
    //TODO implement QRectF type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QLine>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QLine type
    throw QString("QLine type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QLine>::getJson(){
    //TODO implement QLine type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QTextLength>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QTextLength type
    throw QString("QTextLength type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QTextLength>::getJson(){
    //TODO implement QTextLength type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QStringList>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QStringList type
    throw QString("QStringList type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QStringList>::getJson(){
    QJsonArray array;
    for(const QString& string: *value){
        array.append(string);
    }
    return array;
}

template<>
ArgumentImplementation<QVariantMap>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVariantMap type
    throw QString("QVariantMap type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QVariantMap>::getJson(){
    //TODO implement QVariantMap type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QVariantHash>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVariantHash type
    throw QString("QVariantHash type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QVariantHash>::getJson(){
    //TODO implement QVariantHash type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QIcon>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QIcon type
    throw QString("QIcon type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QIcon>::getJson(){
    //TODO implement QIcon type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QPen>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPen type
    throw QString("QPen type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QPen>::getJson(){
    //TODO implement QPen type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QLineF>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QLineF type
    throw QString("QLineF type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QLineF>::getJson(){
    //TODO implement QLineF type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QTextFormat>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QTextFormat type
    throw QString("QTextFormat type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QTextFormat>::getJson(){
    //TODO implement QTextFormat type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QRect>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QRect type
    throw QString("QRect type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QRect>::getJson(){
    //TODO implement QRect type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QPoint>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPoint type
    throw QString("QPoint type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QPoint>::getJson(){
    //TODO implement QPoint type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QUrl>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isString() ){
        QUrl url(argument.toString(), QUrl::ParsingMode::StrictMode);
        if(url.isValid()){
            setValue(url);
        }else{
            throw QString("Cannot convert string to QUrl, because it is formatted incorrectly");
        }

    }else{
        throw QString("Parameter of type %1 cannot be converted to text").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<QUrl>::getJson(){
    //TODO implement QUrl type
    return value->toString();
}

template<>
ArgumentImplementation<QRegExp>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QRegExp type
    throw QString("QRegExp type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QRegExp>::getJson(){
    //TODO implement QRegExp type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QRegularExpression>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QRegularExpression type
    throw QString("QRegularExpression type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QRegularExpression>::getJson(){
    //TODO implement QRegularExpression type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QDateTime>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QDateTime type
    throw QString("QDateTime type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QDateTime>::getJson(){
    return value->toString(Qt::ISODate);
}

template<>
ArgumentImplementation<QPointF>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPointF type
    throw QString("QPointF type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QPointF>::getJson(){
    //TODO implement QPointF type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QPalette>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPalette type
    throw QString("QPalette type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QPalette>::getJson(){
    //TODO implement QPalette type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QFont>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QFont type
    throw QString("QFont type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QFont>::getJson(){
    //TODO implement QFont type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QBrush>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QBrush type
    throw QString("QBrush type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QBrush>::getJson(){
    //TODO implement QBrush type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QRegion>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QRegion type
    throw QString("QRegion type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QRegion>::getJson(){
    //TODO implement QRegion type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QBitArray>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QBitArray type
    throw QString("QBitArray type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QBitArray>::getJson(){
    //TODO implement QBitArray type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QImage>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QImage type
    throw QString("QImage type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QImage>::getJson(){
    //TODO implement QImage type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QKeySequence>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QKeySequence type
    throw QString("QKeySequence type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QKeySequence>::getJson(){
    //TODO implement QKeySequence type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QSizePolicy>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QSizePolicy type
    throw QString("QSizePolicy type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QSizePolicy>::getJson(){
    //TODO implement QSizePolicy type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QPixmap>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPixmap type
    throw QString("QPixmap type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QPixmap>::getJson(){
    //TODO implement QPixmap type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QLocale>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QLocale type
    throw QString("QLocale type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QLocale>::getJson(){
    //TODO implement QLocale type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QBitmap>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QBitmap type
    throw QString("QBitmap type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QBitmap>::getJson(){
    //TODO implement QBitmap type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QMatrix>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QMatrix type
    throw QString("QMatrix type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QMatrix>::getJson(){
    //TODO implement QMatrix type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QTransform>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QTransform type
    throw QString("QTransform type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QTransform>::getJson(){
    //TODO implement QTransform type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QMatrix4x4>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QMatrix4x4 type
    throw QString("QMatrix4x4 type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QMatrix4x4>::getJson(){
    //TODO implement QMatrix4x4 type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QVector2D>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVector2D type
    throw QString("QVector2D type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QVector2D>::getJson(){
    //TODO implement QVector2D type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QVector3D>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVector3D type
    throw QString("QVector3D type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QVector3D>::getJson(){
    //TODO implement QVector3D type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QVector4D>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVector4D type
    throw QString("QVector4D type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QVector4D>::getJson(){
    //TODO implement QVector4D type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QQuaternion>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QQuaternion type
    throw QString("QQuaternion type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QQuaternion>::getJson(){
    //TODO implement QQuaternion type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QEasingCurve>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QEasingCurve type
    throw QString("QEasingCurve type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QEasingCurve>::getJson(){
    //TODO implement QEasingCurve type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QJsonValue>::ArgumentImplementation(const QJsonValue& argument){
    //TODO What if QJsonValue::Undefined gets passed
    setValue(argument);
}

template<>
QJsonValue ArgumentImplementation<QJsonValue>::getJson(){
    return *value;
}

template<>
ArgumentImplementation<QJsonObject>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isObject() ){
        setValue(argument.toObject());
    }else{
        throw QString("Parameter of type %1 cannot be converted to object").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<QJsonObject>::getJson(){
    return *value;
}

template<>
ArgumentImplementation<QJsonArray>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isArray() ){
        setValue(argument.toArray());
    }else{
        throw QString("Parameter of type %1 cannot be converted to array").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<QJsonArray>::getJson(){
    return *value;
}

template<>
ArgumentImplementation<QJsonDocument>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isArray() ){
        setValue(QJsonDocument(argument.toArray()));
    }if( argument.isObject() ){
        setValue(QJsonDocument(argument.toObject()));
    }if( argument.isNull() ){
        setValue(QJsonDocument());
    }if( argument.isString() ){
        QJsonDocument document = QJsonDocument::fromJson(argument.toString().toUtf8());
        if(!document.isNull()){
            setValue(document);
        }else{
            throw QString("Cannot convert string to QJsonDocument, because it is formatted incorrectly");
        }

    }else{
        throw QString("Parameter of type %1 cannot be converted to array").arg(QString(argument.type()));
    }
}

template<>
QJsonValue ArgumentImplementation<QJsonDocument>::getJson(){
    if(value->isArray()){
        return value->array();
    }else if(value->isObject()){
        return value->object();
    }else if(value->isNull()){
        return QJsonValue::Null;
    }else {
        return "";
    }
}

template<>
ArgumentImplementation<QCborValue>::ArgumentImplementation(const QJsonValue& argument){
    //TODO test for all types
    setValue(QCborValue::fromJsonValue(argument));
}

template<>
QJsonValue ArgumentImplementation<QCborValue>::getJson(){
    return value->toJsonValue();
}

template<>
ArgumentImplementation<QCborArray>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QCborArray type
    throw QString("QCborArray type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QCborArray>::getJson(){
    //TODO implement QCborArray type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QCborMap>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QCborMap type
    throw QString("QCborMap type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QCborMap>::getJson(){
    //TODO implement QCborMap type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QCborSimpleType>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QCborSimpleType type
    throw QString("QCborSimpleType type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QCborSimpleType>::getJson(){
    //TODO implement QCborSimpleType type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QModelIndex>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QModelIndex type
    throw QString("QModelIndex type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QModelIndex>::getJson(){
    //TODO implement QModelIndex type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QPersistentModelIndex>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPersistentModelIndex type
    throw QString("QPersistentModelIndex type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QPersistentModelIndex>::getJson(){
    //TODO implement QPersistentModelIndex type
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<QUuid>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isString() ){
        QUuid uuid(argument.toString());
        if(!uuid.isNull()){
            setValue(uuid);
        }else{
            throw QString("Cannot convert string to QUuid, because it is formatted incorrectly");
        }
    }else{
        throw QString("Parameter of type %1 cannot be converted to text").arg(QString(argument.type()));
    }
}


template<>
QJsonValue ArgumentImplementation<QUuid>::getJson(){
    return value->toString();
}

template<>
ArgumentImplementation<QByteArrayList>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QByteArrayList type
    throw QString("QByteArrayList type not implemented");
}

template<>
QJsonValue ArgumentImplementation<QByteArrayList>::getJson(){
    //TODO implement QByteArrayList type
    return QJsonValue::Undefined;
}

template<typename T>
ArgumentImplementation<T>::~ArgumentImplementation()
{
    delete value;
}

template<typename T>
ArgumentImplementation<T>::ArgumentImplementation()
{
    value = new T();
}

Argument *Argument::create(const int requiredTypeId, const QJsonValue &value)
{
    switch((QMetaType::Type)requiredTypeId){
    case QMetaType::Void :
        //TODO decide how to handle void
        throw QString("void not implemented");
        break;
    case QMetaType::UnknownType :
        //Target type unregistered
        throw QString("Unknown/Unregistered target type");
        break;
    case QMetaType::Bool :
        return createArgument<bool>(value);
        break;
    case QMetaType::Int :
        return createArgument<int>(value);
        break;
    case QMetaType::UInt :
        return createArgument<unsigned int>(value);
        break;
    case QMetaType::Double :
        return createArgument<double>(value);
        break;
    case QMetaType::QChar :
        return createArgument<QChar>(value);
        break;
    case QMetaType::QString :
        return createArgument<QString>(value);
        break;
    case QMetaType::QByteArray :
        return createArgument<QByteArray>(value);
        break;
    case QMetaType::Nullptr :
        return createArgument<std::nullptr_t>(value);
        break;
    case QMetaType::VoidStar :
        return createArgument<void*>(value);
        break;
    case QMetaType::Long :
        return createArgument<long>(value);
        break;
    case QMetaType::LongLong :
        return createArgument<long long>(value);
        break;
    case QMetaType::Short :
        return createArgument<short>(value);
        break;
    case QMetaType::Char :
        return createArgument<char>(value);
        break;
    case QMetaType::ULong :
        return createArgument<unsigned long>(value);
        break;
    case QMetaType::ULongLong :
        return createArgument<unsigned long long>(value);
        break;
    case QMetaType::UShort :
        return createArgument<unsigned short>(value);
        break;
    case QMetaType::SChar :
        return createArgument<signed char>(value);
        break;
    case QMetaType::UChar :
        return createArgument<unsigned char>(value);
        break;
    case QMetaType::Float :
        return createArgument<float>(value);
        break;
    case QMetaType::QObjectStar :
        return createArgument<QObject*>(value);
        break;
    case QMetaType::QVariant :
        return createArgument<QVariant>(value);
        break;
    case QMetaType::QCursor :
        return createArgument<QCursor>(value);
        break;
    case QMetaType::QDate :
        return createArgument<QDate>(value);
        break;
    case QMetaType::QSize :
        return createArgument<QSize>(value);
        break;
    case QMetaType::QTime :
        return createArgument<QTime>(value);
        break;
    case QMetaType::QVariantList :
        return createArgument<QVariantList>(value);
        break;
    case QMetaType::QPolygon :
        return createArgument<QPolygon>(value);
        break;
    case QMetaType::QPolygonF :
        return createArgument<QPolygonF>(value);
        break;
    case QMetaType::QColor :
        return createArgument<QColor>(value);
        break;
    case QMetaType::QColorSpace :
        return createArgument<QColorSpace>(value);
        break;
    case QMetaType::QSizeF :
        return createArgument<QSizeF>(value);
        break;
    case QMetaType::QRectF :
        return createArgument<QRectF>(value);
        break;
    case QMetaType::QLine :
        return createArgument<QLine>(value);
        break;
    case QMetaType::QTextLength :
        return createArgument<QTextLength>(value);
        break;
    case QMetaType::QStringList :
        return createArgument<QStringList>(value);
        break;
    case QMetaType::QVariantMap :
        return createArgument<QVariantMap>(value);
        break;
    case QMetaType::QVariantHash :
        return createArgument<QVariantHash>(value);
        break;
    case QMetaType::QIcon :
        return createArgument<QIcon>(value);
        break;
    case QMetaType::QPen :
        return createArgument<QPen>(value);
        break;
    case QMetaType::QLineF :
        return createArgument<QLineF>(value);
        break;
    case QMetaType::QTextFormat :
        return createArgument<QTextFormat>(value);
        break;
    case QMetaType::QRect :
        return createArgument<QRect>(value);
        break;
    case QMetaType::QPoint :
        return createArgument<QPoint>(value);
        break;
    case QMetaType::QUrl :
        return createArgument<QUrl>(value);
        break;
    case QMetaType::QRegExp :
        return createArgument<QRegExp>(value);
        break;
    case QMetaType::QRegularExpression :
        return createArgument<QRegularExpression>(value);
        break;
    case QMetaType::QDateTime :
        return createArgument<QDateTime>(value);
        break;
    case QMetaType::QPointF :
        return createArgument<QPointF>(value);
        break;
    case QMetaType::QPalette :
        return createArgument<QPalette>(value);
        break;
    case QMetaType::QFont :
        return createArgument<QFont>(value);
        break;
    case QMetaType::QBrush :
        return createArgument<QBrush>(value);
        break;
    case QMetaType::QRegion :
        return createArgument<QRegion>(value);
        break;
    case QMetaType::QBitArray :
        return createArgument<QBitArray>(value);
        break;
    case QMetaType::QImage :
        return createArgument<QImage>(value);
        break;
    case QMetaType::QKeySequence :
        return createArgument<QKeySequence>(value);
        break;
    case QMetaType::QSizePolicy :
        return createArgument<QSizePolicy>(value);
        break;
    case QMetaType::QPixmap :
        return createArgument<QPixmap>(value);
        break;
    case QMetaType::QLocale :
        return createArgument<QLocale>(value);
        break;
    case QMetaType::QBitmap :
        return createArgument<QBitmap>(value);
        break;
    case QMetaType::QMatrix :
        return createArgument<QMatrix>(value);
        break;
    case QMetaType::QTransform :
        return createArgument<QTransform>(value);
        break;
    case QMetaType::QMatrix4x4 :
        return createArgument<QMatrix4x4>(value);
        break;
    case QMetaType::QVector2D :
        return createArgument<QVector2D>(value);
        break;
    case QMetaType::QVector3D :
        return createArgument<QVector3D>(value);
        break;
    case QMetaType::QVector4D :
        return createArgument<QVector4D>(value);
        break;
    case QMetaType::QQuaternion :
        return createArgument<QQuaternion>(value);
        break;
    case QMetaType::QEasingCurve :
        return createArgument<QEasingCurve>(value);
        break;
    case QMetaType::QJsonValue :
        return createArgument<QJsonValue>(value);
        break;
    case QMetaType::QJsonObject :
        return createArgument<QJsonObject>(value);
        break;
    case QMetaType::QJsonArray :
        return createArgument<QJsonArray>(value);
        break;
    case QMetaType::QJsonDocument :
        return createArgument<QJsonDocument>(value);
        break;
    case QMetaType::QCborValue :
        return createArgument<QCborValue>(value);
        break;
    case QMetaType::QCborArray :
        return createArgument<QCborArray>(value);
        break;
    case QMetaType::QCborMap :
        return createArgument<QCborMap>(value);
        break;
    case QMetaType::QCborSimpleType :
        return createArgument<QCborSimpleType>(value);
        break;
    case QMetaType::QModelIndex :
        return createArgument<QModelIndex>(value);
        break;
    case QMetaType::QPersistentModelIndex :
        return createArgument<QPersistentModelIndex>(value);
        break;
    case QMetaType::QUuid :
        return createArgument<QUuid>(value);
        break;
    case QMetaType::QByteArrayList :
        return createArgument<QByteArrayList>(value);
        break;
    default:
        //Usertype
        throw QString("Support for usertypes not implemented");
    }
}

template<typename T>
Argument *Argument::createArgument(const QJsonValue &jsonValue)
{
    if(jsonValue != QJsonValue::Undefined){
        return new ArgumentImplementation<T>(jsonValue);
    }else{
        return new ArgumentImplementation<T>();
    }
}
