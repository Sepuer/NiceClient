#ifndef NETENGINE_H
#define NETENGINE_H

#include <QObject>
#include <QThread>
#include "Def.h"
#include "NetSocket.h"

class NetEngine : public QObject
{
    Q_OBJECT
public:
    explicit NetEngine(QObject *parent = nullptr);
    ~NetEngine();
    void cliStart(const CLI_CONF conf);
    void connSvr(const QString ip, const quint16 nPort);
    void sendData(QByteArray baData);
    void abortSock();

private:
    //创建通信线程
    void startSockThd(const quint16 nThd);
signals:
    void sign_connSvr(QString, quint16, int);
    void sign_abortSock();
    void sign_sendData(QByteArray);
    void sign_output(QObject *, QByteArray);
    void sign_count(int);
public slots:
    //接收数据
    void slot_recvData(QObject *pSock, QByteArray baData);
    //成功连接
    void slot_connected(QObject *pSock);
    //释放资源
    void slot_dealloc(QObject *pSock);
    //成功发送
    void slot_writeReady(QObject *pSock);


private:
    CLI_CONF            m_tCliConf;
    QList<NetSocket *>  m_lpNetSockThd;
    QList<QThread *>    m_lpSockThd;
};

#endif // NETENGINE_H
