#ifndef NETENGINE_H
#define NETENGINE_H

#include <QObject>

class NetEngine : public QObject
{
    Q_OBJECT
public:
    explicit NetEngine(QObject *parent = nullptr);

signals:

public slots:
};

#endif // NETENGINE_H
