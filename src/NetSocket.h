#ifndef NETSOCKET_H
#define NETSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QMutex>

class NetSocket : public QObject
{
    Q_OBJECT
public:
    explicit NetSocket(QObject *parent = nullptr);

signals:
    void sign_readReady(QObject *, QByteArray);
    void sign_dealloc(QObject *);       //释放资源
    void sign_connected(QObject *);
    void sign_writeReady(QObject *);
public slots:
    void slot_connSvr(const QString ip, const quint16 nPort, const int nNub=1);
    void slot_connected();
    void slot_disconnected();
    void slot_readReady();
    void slot_sendData(QByteArray baData);
    void slot_abortSock();              //断开所有连接

private:
    QList<QTcpSocket *>     m_lSock;
    QMutex                  m_mutexSock;
};

#endif // NETSOCKET_H
