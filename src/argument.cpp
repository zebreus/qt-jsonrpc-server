#include "argument.h"

QGenericArgument Argument::getArgument()
{
    return argument;
}

Argument::~Argument()
{

}

Argument::Argument()
{

}

template<typename T>
void ArgumentImplementation<T>::setValue(const T& value){
    T* valuePointer = new T(value);
    this->value.reset(valuePointer);
    argument = Q_ARG(T, *valuePointer);
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
ArgumentImplementation<int>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw "Cannot convert number to int, because it is not integer";
        }
        if (integerPart > (double)INT_MAX || integerPart < (double)INT_MIN){
            throw "Cannot convert number to int, because it is out of range";
        }
        setValue((int)integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<unsigned int>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw "Cannot convert number to unsigned int, because it is not integer";
        }
        if (integerPart > (double)UINT_MAX || integerPart < (double)0){
            throw "Cannot convert number to unsigned int, because it is out of range";
        }
        setValue((unsigned int)integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<double>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        setValue(argument.toDouble());
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

//TODO char from int
template<>
ArgumentImplementation<QChar>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isString() ){
        QString stringValue = argument.toString();
        if(stringValue.length() != 1){
            throw "Cannot convert string to char, because it is not one char long";
        }
        setValue(stringValue.front());
    }else{
        throw QString("Parameter of type %1 cannot be converted to text").arg(QString(argument.type()));
    }
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
ArgumentImplementation<QByteArray>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isString() ){
        //TODO Check if toUtf8 is appropriate
        setValue(argument.toString().toUtf8());
    }else{
        throw QString("Parameter of type %1 cannot be converted to text").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<std::nullptr_t>::ArgumentImplementation(const QJsonValue& argument){
    //TODO nullptr_t is probably best represented by null in json, maybe also undefined idk
    if( argument.isNull() ){
        setValue(nullptr);
    }else{
        throw QString("Parameter of type %1 cannot be converted to text").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<void*>::ArgumentImplementation(const QJsonValue& argument){
    //voidpointer will always fails
    throw QString("Parameter of type %1 cannot be converted to void pointer").arg(QString(argument.type()));
}

template<>
ArgumentImplementation<long>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw "Cannot convert number to long, because it is not integer";
        }
        if (integerPart > (double)LONG_MAX || integerPart < (double)LONG_MIN){
            throw "Cannot convert number to long, because it is out of range";
        }
        setValue((long)integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<long long>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw "Cannot convert number to long long, because it is not integer";
        }
        if (integerPart > (double)LONG_LONG_MAX || integerPart < (double)LONG_LONG_MIN){
            throw "Cannot convert number to long long, because it is out of range";
        }
        setValue((long long)integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<short>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw "Cannot convert number to short, because it is not integer";
        }
        if (integerPart > (double)SHRT_MAX || integerPart < (double)SHRT_MIN){
            throw "Cannot convert number to short, because it is out of range";
        }
        setValue(integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<char>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw "Cannot convert number to char, because it is not integer";
        }
        if (integerPart > (double)CHAR_MAX || integerPart < (double)CHAR_MIN){
            throw "Cannot convert number to char, because it is out of range";
        }
        setValue(integerPart);
    }else if( argument.isString() ){
        QByteArray charArray = argument.toString().toUtf8();
        if(charArray.length() != 1){
            throw "Cannot convert string to char, because it is not one char long";
        }
        setValue(charArray.at(0));
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<unsigned long>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw "Cannot convert number to unsigned long, because it is not integer";
        }
        if (integerPart > (double)ULONG_MAX || integerPart < (double)0){
            throw "Cannot convert number to unsigned long, because it is out of range";
        }
        setValue(integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<unsigned long long>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw "Cannot convert number to unsigned long long, because it is not integer";
        }
        if (integerPart > (double)ULONG_LONG_MAX || integerPart < (double)0){
            throw "Cannot convert number to unsigned long long, because it is out of range";
        }
        setValue(integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<unsigned short>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw "Cannot convert number to unsigned short, because it is not integer";
        }
        if (integerPart > (double)USHRT_MAX || integerPart < (double)0){
            throw "Cannot convert number to unsigned short, because it is out of range";
        }
        setValue(integerPart);
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<signed char>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw "Cannot convert number to unsigned char, because it is not integer";
        }
        if (integerPart > (double)SCHAR_MAX || integerPart < (double)SCHAR_MIN){
            throw "Cannot convert number to unsigned char, because it is out of range";
        }
        setValue(integerPart);
    }else if( argument.isString() ){
        QByteArray charArray = argument.toString().toUtf8();
        if(charArray.length() != 1){
            throw "Cannot convert string to char, because it is not one char long";
        }
        setValue(charArray.at(0));
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<unsigned char>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isDouble() ){
        double doubleValue = argument.toDouble();
        double integerPart;
        if(std::modf(doubleValue, &integerPart) != 0.0){
            throw "Cannot convert number to unsigned char, because it is not integer";
        }
        if (integerPart > (double)UCHAR_MAX || integerPart < (double)0){
            throw "Cannot convert number to unsigned char, because it is out of range";
        }
        setValue(integerPart);
    }else if( argument.isString() ){
        QByteArray charArray = argument.toString().toUtf8();
        if(charArray.length() != 1){
            throw "Cannot convert string to char, because it is not one char long";
        }
        setValue(charArray.at(0));
    }else{
        throw QString("Parameter of type %1 cannot be converted to number").arg(QString(argument.type()));
    }
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
ArgumentImplementation<QObject*>::ArgumentImplementation(const QJsonValue& argument){
    //pointers will always fails
    throw QString("Parameter of type %1 cannot be converted to QObject pointer").arg(QString(argument.type()));
}

template<>
ArgumentImplementation<QVariant>::ArgumentImplementation(const QJsonValue& argument){
    //TODO maybe check variant content
    setValue(argument.toVariant());
}

template<>
ArgumentImplementation<QCursor>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QCursor type
    throw QString("QCursor type not implemented");
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
                throw "Cannot convert string to QDate, because it is formatted incorrectly. Use TextDate or ISODate.";
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
                    throw "Cannot convert array to QDate, because it's values are out of range.";
                }
            }else{
                throw "Cannot convert array to QDate, because it's values are not integers.";
            }
        }else{
            throw "Cannot convert array to QDate, because it has the wrong length.";
        }
    }else{
        throw QString("Parameter of type %1 cannot be converted to date. Required format \"ddd MMM d yyyy\"").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<QSize>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QSize type
    throw QString("QSize type not implemented");
}

template<>
ArgumentImplementation<QTime>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QTime type
    throw QString("QTime type not implemented");
}

template<>
ArgumentImplementation<QVariantList>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVariantList type
    throw QString("QVariantList type not implemented");
}

template<>
ArgumentImplementation<QPolygon>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPolygon type
    throw QString("QPolygon type not implemented");
}

template<>
ArgumentImplementation<QPolygonF>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPolygonF type
    throw QString("QPolygonF type not implemented");
}

template<>
ArgumentImplementation<QColor>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QColor type
    throw QString("QColor type not implemented");
}

template<>
ArgumentImplementation<QColorSpace>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QColorSpace type
    throw QString("QColorSpace type not implemented");
}

template<>
ArgumentImplementation<QSizeF>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QSizeF type
    throw QString("QSizeF type not implemented");
}

template<>
ArgumentImplementation<QRectF>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QRectF type
    throw QString("QRectF type not implemented");
}

template<>
ArgumentImplementation<QLine>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QLine type
    throw QString("QLine type not implemented");
}

template<>
ArgumentImplementation<QTextLength>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QTextLength type
    throw QString("QTextLength type not implemented");
}

template<>
ArgumentImplementation<QStringList>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QStringList type
    throw QString("QStringList type not implemented");
}

template<>
ArgumentImplementation<QVariantMap>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVariantMap type
    throw QString("QVariantMap type not implemented");
}

template<>
ArgumentImplementation<QVariantHash>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVariantHash type
    throw QString("QVariantHash type not implemented");
}

template<>
ArgumentImplementation<QIcon>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QIcon type
    throw QString("QIcon type not implemented");
}

template<>
ArgumentImplementation<QPen>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPen type
    throw QString("QPen type not implemented");
}

template<>
ArgumentImplementation<QLineF>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QLineF type
    throw QString("QLineF type not implemented");
}

template<>
ArgumentImplementation<QTextFormat>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QTextFormat type
    throw QString("QTextFormat type not implemented");
}

template<>
ArgumentImplementation<QRect>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QRect type
    throw QString("QRect type not implemented");
}

template<>
ArgumentImplementation<QPoint>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPoint type
    throw QString("QPoint type not implemented");
}

template<>
ArgumentImplementation<QUrl>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isString() ){
        QUrl url(argument.toString(), QUrl::ParsingMode::StrictMode);
        if(url.isValid()){
            setValue(url);
        }else{
            throw "Cannot convert string to QUrl, because it is formatted incorrectly";
        }

    }else{
        throw QString("Parameter of type %1 cannot be converted to text").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<QRegExp>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QRegExp type
    throw QString("QRegExp type not implemented");
}

template<>
ArgumentImplementation<QRegularExpression>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QRegularExpression type
    throw QString("QRegularExpression type not implemented");
}

template<>
ArgumentImplementation<QDateTime>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QDateTime type
    throw QString("QDateTime type not implemented");
}

template<>
ArgumentImplementation<QPointF>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPointF type
    throw QString("QPointF type not implemented");
}

template<>
ArgumentImplementation<QPalette>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPalette type
    throw QString("QPalette type not implemented");
}

template<>
ArgumentImplementation<QFont>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QFont type
    throw QString("QFont type not implemented");
}

template<>
ArgumentImplementation<QBrush>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QBrush type
    throw QString("QBrush type not implemented");
}

template<>
ArgumentImplementation<QRegion>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QRegion type
    throw QString("QRegion type not implemented");
}

template<>
ArgumentImplementation<QBitArray>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QBitArray type
    throw QString("QBitArray type not implemented");
}

template<>
ArgumentImplementation<QImage>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QImage type
    throw QString("QImage type not implemented");
}

template<>
ArgumentImplementation<QKeySequence>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QKeySequence type
    throw QString("QKeySequence type not implemented");
}

template<>
ArgumentImplementation<QSizePolicy>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QSizePolicy type
    throw QString("QSizePolicy type not implemented");
}

template<>
ArgumentImplementation<QPixmap>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPixmap type
    throw QString("QPixmap type not implemented");
}

template<>
ArgumentImplementation<QLocale>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QLocale type
    throw QString("QLocale type not implemented");
}

template<>
ArgumentImplementation<QBitmap>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QBitmap type
    throw QString("QBitmap type not implemented");
}

template<>
ArgumentImplementation<QMatrix>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QMatrix type
    throw QString("QMatrix type not implemented");
}

template<>
ArgumentImplementation<QTransform>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QTransform type
    throw QString("QTransform type not implemented");
}

template<>
ArgumentImplementation<QMatrix4x4>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QMatrix4x4 type
    throw QString("QMatrix4x4 type not implemented");
}

template<>
ArgumentImplementation<QVector2D>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVector2D type
    throw QString("QVector2D type not implemented");
}

template<>
ArgumentImplementation<QVector3D>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVector3D type
    throw QString("QVector3D type not implemented");
}

template<>
ArgumentImplementation<QVector4D>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QVector4D type
    throw QString("QVector4D type not implemented");
}

template<>
ArgumentImplementation<QQuaternion>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QQuaternion type
    throw QString("QQuaternion type not implemented");
}

template<>
ArgumentImplementation<QEasingCurve>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QEasingCurve type
    throw QString("QEasingCurve type not implemented");
}

template<>
ArgumentImplementation<QJsonValue>::ArgumentImplementation(const QJsonValue& argument){
    setValue(argument);
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
ArgumentImplementation<QJsonArray>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isArray() ){
        setValue(argument.toArray());
    }else{
        throw QString("Parameter of type %1 cannot be converted to array").arg(QString(argument.type()));
    }
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
            throw "Cannot convert string to QJsonDocument, because it is formatted incorrectly";
        }

    }else{
        throw QString("Parameter of type %1 cannot be converted to array").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<QCborValue>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QCborValue type
    throw QString("QCborValue type not implemented");
}

template<>
ArgumentImplementation<QCborArray>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QCborArray type
    throw QString("QCborArray type not implemented");
}

template<>
ArgumentImplementation<QCborMap>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QCborMap type
    throw QString("QCborMap type not implemented");
}

template<>
ArgumentImplementation<QCborSimpleType>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QCborSimpleType type
    throw QString("QCborSimpleType type not implemented");
}

template<>
ArgumentImplementation<QModelIndex>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QModelIndex type
    throw QString("QModelIndex type not implemented");
}

template<>
ArgumentImplementation<QPersistentModelIndex>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QPersistentModelIndex type
    throw QString("QPersistentModelIndex type not implemented");
}

template<>
ArgumentImplementation<QUuid>::ArgumentImplementation(const QJsonValue& argument){
    if( argument.isString() ){
        QUuid uuid(argument.toString());
        if(!uuid.isNull()){
            setValue(uuid);
        }else{
            throw "Cannot convert string to QUuid, because it is formatted incorrectly";
        }
    }else{
        throw QString("Parameter of type %1 cannot be converted to text").arg(QString(argument.type()));
    }
}

template<>
ArgumentImplementation<QByteArrayList>::ArgumentImplementation(const QJsonValue& argument){
    Q_UNUSED(argument);
    //TODO implement QByteArrayList type
    throw QString("QByteArrayList type not implemented");
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
        return new ArgumentImplementation<bool>(value);
        break;
    case QMetaType::Int :
        return new ArgumentImplementation<int>(value);
        break;
    case QMetaType::UInt :
        return new ArgumentImplementation<unsigned int>(value);
        break;
    case QMetaType::Double :
        return new ArgumentImplementation<double>(value);
        break;
    case QMetaType::QChar :
        return new ArgumentImplementation<QChar>(value);
        break;
    case QMetaType::QString :
        return new ArgumentImplementation<QString>(value);
        break;
    case QMetaType::QByteArray :
        return new ArgumentImplementation<QByteArray>(value);
        break;
    case QMetaType::Nullptr :
        return new ArgumentImplementation<std::nullptr_t>(value);
        break;
    case QMetaType::VoidStar :
        return new ArgumentImplementation<void*>(value);
        break;
    case QMetaType::Long :
        return new ArgumentImplementation<long>(value);
        break;
    case QMetaType::LongLong :
        return new ArgumentImplementation<long long>(value);
        break;
    case QMetaType::Short :
        return new ArgumentImplementation<short>(value);
        break;
    case QMetaType::Char :
        return new ArgumentImplementation<char>(value);
        break;
    case QMetaType::ULong :
        return new ArgumentImplementation<unsigned long>(value);
        break;
    case QMetaType::ULongLong :
        return new ArgumentImplementation<unsigned long long>(value);
        break;
    case QMetaType::UShort :
        return new ArgumentImplementation<unsigned short>(value);
        break;
    case QMetaType::SChar :
        return new ArgumentImplementation<signed char>(value);
        break;
    case QMetaType::UChar :
        return new ArgumentImplementation<unsigned char>(value);
        break;
    case QMetaType::Float :
        return new ArgumentImplementation<float>(value);
        break;
    case QMetaType::QObjectStar :
        return new ArgumentImplementation<QObject*>(value);
        break;
    case QMetaType::QVariant :
        return new ArgumentImplementation<QVariant>(value);
        break;
    case QMetaType::QCursor :
        return new ArgumentImplementation<QCursor>(value);
        break;
    case QMetaType::QDate :
        return new ArgumentImplementation<QDate>(value);
        break;
    case QMetaType::QSize :
        return new ArgumentImplementation<QSize>(value);
        break;
    case QMetaType::QTime :
        return new ArgumentImplementation<QTime>(value);
        break;
    case QMetaType::QVariantList :
        return new ArgumentImplementation<QVariantList>(value);
        break;
    case QMetaType::QPolygon :
        return new ArgumentImplementation<QPolygon>(value);
        break;
    case QMetaType::QPolygonF :
        return new ArgumentImplementation<QPolygonF>(value);
        break;
    case QMetaType::QColor :
        return new ArgumentImplementation<QColor>(value);
        break;
    case QMetaType::QColorSpace :
        return new ArgumentImplementation<QColorSpace>(value);
        break;
    case QMetaType::QSizeF :
        return new ArgumentImplementation<QSizeF>(value);
        break;
    case QMetaType::QRectF :
        return new ArgumentImplementation<QRectF>(value);
        break;
    case QMetaType::QLine :
        return new ArgumentImplementation<QLine>(value);
        break;
    case QMetaType::QTextLength :
        return new ArgumentImplementation<QTextLength>(value);
        break;
    case QMetaType::QStringList :
        return new ArgumentImplementation<QStringList>(value);
        break;
    case QMetaType::QVariantMap :
        return new ArgumentImplementation<QVariantMap>(value);
        break;
    case QMetaType::QVariantHash :
        return new ArgumentImplementation<QVariantHash>(value);
        break;
    case QMetaType::QIcon :
        return new ArgumentImplementation<QIcon>(value);
        break;
    case QMetaType::QPen :
        return new ArgumentImplementation<QPen>(value);
        break;
    case QMetaType::QLineF :
        return new ArgumentImplementation<QLineF>(value);
        break;
    case QMetaType::QTextFormat :
        return new ArgumentImplementation<QTextFormat>(value);
        break;
    case QMetaType::QRect :
        return new ArgumentImplementation<QRect>(value);
        break;
    case QMetaType::QPoint :
        return new ArgumentImplementation<QPoint>(value);
        break;
    case QMetaType::QUrl :
        return new ArgumentImplementation<QUrl>(value);
        break;
    case QMetaType::QRegExp :
        return new ArgumentImplementation<QRegExp>(value);
        break;
    case QMetaType::QRegularExpression :
        return new ArgumentImplementation<QRegularExpression>(value);
        break;
    case QMetaType::QDateTime :
        return new ArgumentImplementation<QDateTime>(value);
        break;
    case QMetaType::QPointF :
        return new ArgumentImplementation<QPointF>(value);
        break;
    case QMetaType::QPalette :
        return new ArgumentImplementation<QPalette>(value);
        break;
    case QMetaType::QFont :
        return new ArgumentImplementation<QFont>(value);
        break;
    case QMetaType::QBrush :
        return new ArgumentImplementation<QBrush>(value);
        break;
    case QMetaType::QRegion :
        return new ArgumentImplementation<QRegion>(value);
        break;
    case QMetaType::QBitArray :
        return new ArgumentImplementation<QBitArray>(value);
        break;
    case QMetaType::QImage :
        return new ArgumentImplementation<QImage>(value);
        break;
    case QMetaType::QKeySequence :
        return new ArgumentImplementation<QKeySequence>(value);
        break;
    case QMetaType::QSizePolicy :
        return new ArgumentImplementation<QSizePolicy>(value);
        break;
    case QMetaType::QPixmap :
        return new ArgumentImplementation<QPixmap>(value);
        break;
    case QMetaType::QLocale :
        return new ArgumentImplementation<QLocale>(value);
        break;
    case QMetaType::QBitmap :
        return new ArgumentImplementation<QBitmap>(value);
        break;
    case QMetaType::QMatrix :
        return new ArgumentImplementation<QMatrix>(value);
        break;
    case QMetaType::QTransform :
        return new ArgumentImplementation<QTransform>(value);
        break;
    case QMetaType::QMatrix4x4 :
        return new ArgumentImplementation<QMatrix4x4>(value);
        break;
    case QMetaType::QVector2D :
        return new ArgumentImplementation<QVector2D>(value);
        break;
    case QMetaType::QVector3D :
        return new ArgumentImplementation<QVector3D>(value);
        break;
    case QMetaType::QVector4D :
        return new ArgumentImplementation<QVector4D>(value);
        break;
    case QMetaType::QQuaternion :
        return new ArgumentImplementation<QQuaternion>(value);
        break;
    case QMetaType::QEasingCurve :
        return new ArgumentImplementation<QEasingCurve>(value);
        break;
    case QMetaType::QJsonValue :
        return new ArgumentImplementation<QJsonValue>(value);
        break;
    case QMetaType::QJsonObject :
        return new ArgumentImplementation<QJsonObject>(value);
        break;
    case QMetaType::QJsonArray :
        return new ArgumentImplementation<QJsonArray>(value);
        break;
    case QMetaType::QJsonDocument :
        return new ArgumentImplementation<QJsonDocument>(value);
        break;
    case QMetaType::QCborValue :
        return new ArgumentImplementation<QCborValue>(value);
        break;
    case QMetaType::QCborArray :
        return new ArgumentImplementation<QCborArray>(value);
        break;
    case QMetaType::QCborMap :
        return new ArgumentImplementation<QCborMap>(value);
        break;
    case QMetaType::QCborSimpleType :
        return new ArgumentImplementation<QCborSimpleType>(value);
        break;
    case QMetaType::QModelIndex :
        return new ArgumentImplementation<QModelIndex>(value);
        break;
    case QMetaType::QPersistentModelIndex :
        return new ArgumentImplementation<QPersistentModelIndex>(value);
        break;
    case QMetaType::QUuid :
        return new ArgumentImplementation<QUuid>(value);
        break;
    case QMetaType::QByteArrayList :
        return new ArgumentImplementation<QByteArrayList>(value);
        break;
    default:
        //Usertype
        throw "Support for usertypes not implemented";
    }
}

