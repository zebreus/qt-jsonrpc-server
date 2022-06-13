#ifndef CLIENT_H
#define CLIENT_H

#include <QDebug>
#include <QMetaObject>
#include <QObject>
#include <QUrl>

template<typename T>
concept QObjectWithMacro = std::is_base_of<T, QObject>::value&& requires(T a) {
  { T::staticMetaObject }
  ->std::convertible_to<QMetaObject>;
};

template<QObjectWithMacro T>
class Client: public T {
 public:
  explicit Client(const QUrl& serverUrl, QObject* parent = nullptr);

  static QMetaObject staticMetaObject;
  virtual const QMetaObject* metaObject() const;
  virtual void* qt_metacast(const char*);
  virtual int qt_metacall(QMetaObject::Call, int, void**);
  static inline QString tr(const char* s, const char* c = nullptr, int n = -1);
  static inline QString trUtf8(const char* s, const char* c = nullptr, int n = -1);

 private:
  static void qt_static_metacall(QObject*, QMetaObject::Call, int, void**);

 signals:
};

template<QObjectWithMacro T>
inline void Client<T>::qt_static_metacall(QObject* target, QMetaObject::Call call, int id, void** arguments) {
  qDebug() << "My qt_static_metacall";
  return T::qt_static_metacall(target, call, id, arguments);
}

template<QObjectWithMacro T>
inline const QMetaObject* Client<T>::metaObject() const {
  qDebug() << "My metaObject " << (QObject::d_ptr->metaObject ? "QObject::d_ptr->dynamicMetaObject()" : "&staticMetaObject");
  return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

template<QObjectWithMacro T>
inline void* Client<T>::qt_metacast(const char* c) {
  qDebug() << "My qt_metacast (" << c << ")";
  return T::qt_metacast(c);
}

template<QObjectWithMacro T>
inline int Client<T>::qt_metacall(QMetaObject::Call call, int id, void** arguments) {
  qDebug() << "My qt_metacall";
  return T::qt_metacall(call, id, arguments);
}

template<QObjectWithMacro T>
inline QString Client<T>::tr(const char* s, const char* c, int n) {
  return staticMetaObject.tr(s, c, n);
}

template<QObjectWithMacro T>
inline QString Client<T>::trUtf8(const char* s, const char* c, int n) {
  return staticMetaObject.tr(s, c, n);
}

template<QObjectWithMacro T>
inline QMetaObject Client<T>::staticMetaObject = {
    {T::d.superdata, T::d.stringdata, T::d.data, qt_static_metacall, T::d.relatedMetaObject, T::d.extradata}};

/*
static const QMetaObject staticMetaObject;
virtual const QMetaObject *metaObject() const;
virtual void *qt_metacast(const char *);
virtual int qt_metacall(QMetaObject::Call, int, void **);
static inline QString tr(const char *s, const char *c = nullptr, int n = -1) {
    return staticMetaObject.tr(s, c, n);
}
__attribute__ ((__deprecated__))
static inline QString trUtf8(const char *s, const char *c = nullptr, int n = -1) {
    return staticMetaObject.tr(s, c, n);
}
private:
__attribute__((visibility("hidden")))
static void qt_static_metacall(QObject *, QMetaObject::Call, int, void **);
#pragma clang diagnostic pop

struct QPrivateSignal {};
*/

#endif  // CLIENT_H
