#ifndef NETSOCKET_H
#define NETSOCKET_H

#include <QObject>

class NetSocket : public QObject
{
    Q_OBJECT
public:
    explicit NetSocket(QObject *parent = nullptr);

signals:

public slots:
    void slot_connSvr(QString ip, quint16 nPort);
};

#endif // NETSOCKET_H
