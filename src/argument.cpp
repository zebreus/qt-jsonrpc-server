#include "argument.h"

using namespace jsonrpc;

QGenericArgument& Argument::getArgument()
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
        throw exceptions::WrongArgumentType("boolean", argument);
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
            throw exceptions::WrongArgumentType("int", argument, "it is not integer.");
        }
        if (integerPart > (double)INT_MAX || integerPart < (double)INT_MIN){
            throw exceptions::WrongArgumentType("int", argument, "it is out of range.");
        }
        setValue((int)integerPart);
    }else if( argument.isString() ){
        bool ok;
        int number = argument.toString().toInt(&ok);
        if(!ok){
            throw exceptions::WrongArgumentType("int", argument, "it is not a valid int.");
        }else{
            setValue(number);
        }
    }else{
        throw exceptions::WrongArgumentType("int", argument);
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
            throw exceptions::WrongArgumentType("unsigned int", argument, "it is not integer.");
        }
        if (integerPart > (double)UINT_MAX || integerPart < (double)0){
            throw exceptions::WrongArgumentType("unsigned int", argument, "it is out of range.");
        }
        setValue((unsigned int)integerPart);
    }else if( argument.isString() ){
        bool ok;
        unsigned int number = argument.toString().toUInt(&ok);
        if(!ok){
            throw exceptions::WrongArgumentType("unsigned int", argument, "it is not a valid int.");
        }else{
            setValue(number);
        }
    }else{
        throw exceptions::WrongArgumentType("unsigned int", argument);
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
    }else if( argument.isString() ){
        bool ok;
        double number = argument.toString().toDouble(&ok);
        if(!ok){
            throw exceptions::WrongArgumentType("double", argument, "it is not a valid int.");
        }else{
            setValue(number);
        }
    }else{
        throw exceptions::WrongArgumentType("double", argument);
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
        if(stringValue.length() == 0){
            throw exceptions::WrongArgumentType("QChar", argument, "it is empty.");
        }else if(stringValue.length() > 1){
            throw exceptions::WrongArgumentType("QChar", argument, "it is longer than one QChar.");
        }
        setValue(stringValue.front());
    }else if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw exceptions::WrongArgumentType("QChar", argument, "it is not integer.");
        }
        if (integerPart > (double)USHRT_MAX || integerPart < (double)0){
            throw exceptions::WrongArgumentType("QChar", argument, "it is out of range.");
        }
        setValue((unsigned short)integerPart);
    }else{
        throw exceptions::WrongArgumentType("QChar", argument);
    }
}

template<>
QJsonValue ArgumentImplementation<QChar>::getJson(){
    //TODO maybe represent as short string
    return QJsonValue(value->unicode());
}

template<>
ArgumentImplementation<QString>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isString() ){
        setValue(argument.toString());
    }else{
        throw exceptions::WrongArgumentType("QString", argument);
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
                    throw exceptions::WrongArgumentType("byte for QByteArray", argument, "it is not integer.");
                }
                if (integerPart > (double)UCHAR_MAX || integerPart < (double)SCHAR_MIN){
                    throw exceptions::WrongArgumentType("byte for QByteArray", argument, "it is out of range.");
                }
                charPointer[position++] = (char)(int)integerPart;
            }else if( val.isString() ){
                QByteArray charArray = val.toString().toUtf8();
                if(charArray.length() == 0){
                    //TODO maybe an empty string is a nullbyte
                    throw exceptions::WrongArgumentType("byte for QByteArray", argument, "it is empty.");
                }else if(charArray.length() > 1){
                    throw exceptions::WrongArgumentType("byte for QByteArray", argument, "it is longer than one char.");
                }
                charPointer[position++] = charArray.at(0);
            }else{
                throw exceptions::WrongArgumentType("byte for QByteArray", val);
            }
        }
        setValue(QByteArray(charPointer.get(), array.size()));
    }else{
        throw exceptions::WrongArgumentType("QByteArray", argument);
    }
}

template<>
QJsonValue ArgumentImplementation<QByteArray>::getJson(){
    QJsonArray array;
    for(char c: *value){
        array.append(QJsonValue((double)c));
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
            throw exceptions::WrongArgumentType("nullptr_t", argument, "it is not 0.");
        }
    }else{
        throw exceptions::WrongArgumentType("nullptr_t", argument);
    }
}

template<>
QJsonValue ArgumentImplementation<std::nullptr_t>::getJson(){
    return QJsonValue::Null;
}

template<>
ArgumentImplementation<void*>::ArgumentImplementation(const QJsonValue& argument){
    //voidpointer will always fails
    throw exceptions::WrongArgumentType("void pointer", argument, "you would only transfer the pointer and not what it points to.");
}

template<>
QJsonValue ArgumentImplementation<void*>::getJson(){
    return QJsonValue::Undefined;
}

template<>
ArgumentImplementation<long>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw exceptions::WrongArgumentType("long", argument, "it is not integer.");
        }
        if (integerPart > (double)LONG_MAX || integerPart < (double)LONG_MIN){
            throw exceptions::WrongArgumentType("long", argument, "it is out of range.");
        }
        setValue((long)integerPart);
    }else if( argument.isString() ){
        bool ok;
        long number = argument.toString().toLong(&ok);
        if(!ok){
            throw exceptions::WrongArgumentType("long", argument, "it is not a valid int.");
        }else{
            setValue(number);
        }
    }else{
        throw exceptions::WrongArgumentType("long", argument);
    }
}

template<>
QJsonValue ArgumentImplementation<long>::getJson(){
    //All values bigger than 2^52 will be converted to strings, because of double precision.
    if(*value >= std::pow(2,52) || *value <= -std::pow(2,52)){
        return QString::number(*value);
    }else{
        return QJsonValue((double)*value);
    }
}

template<>
ArgumentImplementation<long long>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw exceptions::WrongArgumentType("long long", argument, "it is not integer.");
        }
        if (integerPart > (double)LLONG_MAX || integerPart < (double)LLONG_MIN){
            throw exceptions::WrongArgumentType("long long", argument, "it is out of range.");
        }
        setValue((long long)integerPart);
    }else if( argument.isString() ){
        bool ok;
        long long number = argument.toString().toLongLong(&ok);
        if(!ok){
            throw exceptions::WrongArgumentType("long long", argument, "it is not a valid int.");
        }else{
            setValue(number);
        }
    }else{
        throw exceptions::WrongArgumentType("long long", argument);
    }
}

template<>
QJsonValue ArgumentImplementation<long long>::getJson(){
    //All values bigger than 2^52 will be converted to strings, because of double precision.
    if(*value >= std::pow(2,52) || *value <= -std::pow(2,52)){
        return QString::number(*value);
    }else{
        return QJsonValue((double)*value);
    }
}

template<>
ArgumentImplementation<short>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw exceptions::WrongArgumentType("short", argument, "it is not integer.");
        }
        if (integerPart > (double)SHRT_MAX || integerPart < (double)SHRT_MIN){
            throw exceptions::WrongArgumentType("short", argument, "it is out of range.");
        }
        setValue(integerPart);
    }else if( argument.isString() ){
        bool ok;
        short number = argument.toString().toShort(&ok);
        if(!ok){
            throw exceptions::WrongArgumentType("short", argument, "it is not a valid int.");
        }else{
            setValue(number);
        }
    }else{
        throw exceptions::WrongArgumentType("short", argument);
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
            throw exceptions::WrongArgumentType("char", argument, "it is not integer.");
        }
        if (integerPart > (double)UCHAR_MAX || integerPart < (double)SCHAR_MIN){
            throw exceptions::WrongArgumentType("char", argument, "it is out of range.");
        }
        setValue((char)(int)integerPart);
    }else if( argument.isString() ){
        QByteArray charArray = argument.toString().toUtf8();
        if(charArray.length() == 0){
            //TODO maybe an empty string is a nullbyte
            throw exceptions::WrongArgumentType("char", argument, "it is empty.");
        }else if(charArray.length() > 1){
            throw exceptions::WrongArgumentType("char", argument, "it is longer than one char.");
        }
        setValue(charArray.at(0));
    }else{
        throw exceptions::WrongArgumentType("char", argument);
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
            throw exceptions::WrongArgumentType("unsigned long", argument, "it is not integer.");
        }
        if (integerPart > (double)ULONG_MAX || integerPart < (double)0){
            throw exceptions::WrongArgumentType("unsigned long", argument, "it is out of range.");
        }
        setValue(integerPart);
    }else if( argument.isString() ){
        bool ok;
        unsigned long number = argument.toString().toULong(&ok);
        if(!ok){
            throw exceptions::WrongArgumentType("unsigned long", argument, "it is not a valid int.");
        }else{
            setValue(number);
        }
    }else{
        throw exceptions::WrongArgumentType("unsigned long", argument);
    }
}

template<>
QJsonValue ArgumentImplementation<unsigned long>::getJson(){
    //All values bigger than 2^52 will be converted to strings, because of double precision.
    if(*value >= std::pow(2,52) || *value <= -std::pow(2,52)){
        return QString::number(*value);
    }else{
        return QJsonValue((double)*value);
    }
}

template<>
ArgumentImplementation<unsigned long long>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw exceptions::WrongArgumentType("unsigned long long", argument, "it is not integer.");
        }
        if (integerPart > (double)ULLONG_MAX || integerPart < (double)0){
            throw exceptions::WrongArgumentType("unsigned long long", argument, "it is out of range.");
        }
        setValue(integerPart);
    }else if( argument.isString() ){
        bool ok;
        unsigned long long number = argument.toString().toULongLong(&ok);
        if(!ok){
            throw exceptions::WrongArgumentType("unsigned long long", argument, "it is not a valid int.");
        }else{
            setValue(number);
        }
    }else{
        throw exceptions::WrongArgumentType("unsigned long long", argument);
    }
}

template<>
QJsonValue ArgumentImplementation<unsigned long long>::getJson(){
    //All values bigger than 2^52 will be converted to strings, because of double precision.
    if(*value >= std::pow(2,52) || *value <= -std::pow(2,52)){
        return QString::number(*value);
    }else{
        return QJsonValue((double)*value);
    }
}

template<>
ArgumentImplementation<unsigned short>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw exceptions::WrongArgumentType("unsigned short", argument, "it is not integer.");
        }
        if (integerPart > (double)USHRT_MAX || integerPart < (double)0){
            throw exceptions::WrongArgumentType("unsigned short", argument, "it is out of range.");
        }
        setValue(integerPart);
    }else if( argument.isString() ){
        bool ok;
        unsigned short number = argument.toString().toUShort(&ok);
        if(!ok){
            throw exceptions::WrongArgumentType("unsigned short", argument, "it is not a valid int.");
        }else{
            setValue(number);
        }
    }else{
        throw exceptions::WrongArgumentType("unsigned short", argument);
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
            throw exceptions::WrongArgumentType("signed char", argument, "it is not integer.");
        }
        if (integerPart > (double)SCHAR_MAX || integerPart < (double)SCHAR_MIN){
            throw exceptions::WrongArgumentType("signed char", argument, "it is out of range.");
        }
        setValue(integerPart);
    }else if( argument.isString() ){
        QByteArray charArray = argument.toString().toUtf8();
        if(charArray.length() == 0){
            //TODO maybe an empty string is a nullbyte
            throw exceptions::WrongArgumentType("signed char", argument, "it is empty.");
        }else if(charArray.length() > 1){
            throw exceptions::WrongArgumentType("signed char", argument, "it is longer than one char.");
        }
        setValue(charArray.at(0));
    }else{
        throw exceptions::WrongArgumentType("signed char", argument);
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
            throw exceptions::WrongArgumentType("unsigned char", argument, "it is not integer.");
        }
        if (integerPart > (double)UCHAR_MAX || integerPart < (double)0){
            throw exceptions::WrongArgumentType("unsigned char", argument, "it is out of range.");
        }
        setValue(integerPart);
    }else if( argument.isString() ){
        QByteArray charArray = argument.toString().toUtf8();
        if(charArray.length() == 0){
            //TODO maybe an empty string is a nullbyte
            throw exceptions::WrongArgumentType("unsigned char", argument, "it is empty.");
        }else if(charArray.length() > 1){
            throw exceptions::WrongArgumentType("unsigned char", argument, "it is longer than one char.");
        }
        setValue(charArray.at(0));
    }else{
        throw exceptions::WrongArgumentType("unsigned char", argument);
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
    }else if( argument.isString() ){
        bool ok;
        float number = argument.toString().toFloat(&ok);
        if(!ok){
            throw exceptions::WrongArgumentType("float", argument, "it is not a valid int.");
        }else{
            setValue(number);
        }
    }else{
        throw exceptions::WrongArgumentType("float", argument);
    }
}

template<>
QJsonValue ArgumentImplementation<float>::getJson(){
    return QJsonValue(*value);
}

template<>
ArgumentImplementation<QObject*>::ArgumentImplementation(const QJsonValue& argument){
    //pointers will always fails
    throw exceptions::WrongArgumentType("QObject*", argument, "you would only transfer the pointer and not what it points to.");
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
    throw exceptions::WrongArgumentType("QCursor", argument, "this type is not yet implemented.");
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
                throw exceptions::WrongArgumentType("QDate", argument, "it is formatted incorrectly. Use TextDate or ISODate.");
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
                    throw exceptions::WrongArgumentType("QDate", argument, "the values are out of range.");
                }
            }else{
                throw exceptions::WrongArgumentType("QDate", argument, "the array does not consist of three numbers.");
            }
        }else{
            throw exceptions::WrongArgumentType("QDate", argument, "the array does not consist of three numbers.");
        }
    }else{
        throw exceptions::WrongArgumentType("QDate", argument);
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
    throw exceptions::WrongArgumentType("QSize", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QTime", argument, "this type is not yet implemented.");
}

template<>
QJsonValue ArgumentImplementation<QTime>::getJson(){
    return value->toString("HH:mm:ss");
}

template<>
ArgumentImplementation<QVariantList>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVariantList type
    throw exceptions::WrongArgumentType("QVariantList", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QPolygon", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QPolygonF", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QColor", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QColorSpace", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QSizeF", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QRectF", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QLine", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QTextLength", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QStringList", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QVariantMap", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QVariantHash", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QIcon", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QPen", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QLineF", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QTextFormat", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QRect", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QPoint", argument, "this type is not yet implemented.");
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
            throw exceptions::WrongArgumentType("QUrl", argument, "it is formatted incorrectly.");
        }

    }else{
        throw exceptions::WrongArgumentType("QUrl", argument);
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
    throw exceptions::WrongArgumentType("QRegExp", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QRegularExpression", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QDateTime", argument, "this type is not yet implemented.");
}

template<>
QJsonValue ArgumentImplementation<QDateTime>::getJson(){
    return value->toString(Qt::ISODate);
}

template<>
ArgumentImplementation<QPointF>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPointF type
    throw exceptions::WrongArgumentType("QPointF", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QPalette", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QFont", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QBrush", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QRegion", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QBitArray", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QImage", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QKeySequence", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QSizePolicy", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QPixmap", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QLocale", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QBitmap", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QMatrix", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QTransform", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QMatrix4x4", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QVector2D", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QVector3D", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QVector4D", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QQuaternion", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QEasingCurve", argument, "this type is not yet implemented.");
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
        throw exceptions::WrongArgumentType("QJsonObject", argument);
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
        throw exceptions::WrongArgumentType("QJsonArray", argument);
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
            throw exceptions::WrongArgumentType("QJsonDocument", argument, "it is formatted incorrectly.");
        }

    }else{
        throw exceptions::WrongArgumentType("QJsonDocument", argument);
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
    throw exceptions::WrongArgumentType("QCborArray", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QCborMap", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QCborSimpleType", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QModelIndex", argument, "this type is not yet implemented.");
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
    throw exceptions::WrongArgumentType("QPersistentModelIndex", argument, "this type is not yet implemented.");
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
            throw exceptions::WrongArgumentType("QUuid", argument, "it is formatted incorrectly.");
        }
    }else{
        throw exceptions::WrongArgumentType("QUuid", argument);
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
    throw exceptions::WrongArgumentType("QByteArrayList", argument, "this type is not yet implemented.");
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
    argument = Q_ARG(T, *this->value);
}

Argument *Argument::create(const int requiredTypeId, const QJsonValue &value)
{
    switch((QMetaType::Type)requiredTypeId){
    case QMetaType::Void :
        //TODO decide how to handle void
        throw exceptions::WrongArgumentType("void", value, " void types are not implemented.");
        break;
    case QMetaType::UnknownType :
        //Target type unregistered
        throw exceptions::InvalidSignature("The requested method has an invalid signature.");
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
        QString requiredTypeName = QMetaType::typeName((QMetaType::Type)requiredTypeId);
        throw exceptions::WrongArgumentType(requiredTypeName, value, "user defined types are not yet supported.");
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
