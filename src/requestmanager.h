#ifndef REQUESTMANAGER_H
#define REQUESTMANAGER_H

#include <QObject>

class RequestManager : public QObject
{
    Q_OBJECT
public:
    explicit RequestManager(QObject *parent = nullptr);

signals:

};

#endif // REQUESTMANAGER_H
