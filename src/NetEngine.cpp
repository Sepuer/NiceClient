#include "NetEngine.h"

NetEngine::NetEngine(QObject *parent) : QObject(parent)
{

}

NetEngine::~NetEngine()
{
    emit sign_abortSock();

    for (int i=0; i<m_lpNetSockThd.size(); i++)
    {
        if(m_lpNetSockThd[i] != nullptr)
        {
            m_lpNetSockThd[i]->deleteLater();
            m_lpNetSockThd[i] = nullptr;
        }
    }
    m_lpNetSockThd.clear();
    for (int i=0; i<m_lpSockThd.size(); i++)
    {
        m_lpSockThd[i]->quit();
        m_lpSockThd[i]->wait();
        m_lpSockThd[i]->deleteLater();
    }
    m_lpSockThd.clear();
}

void NetEngine::cliStart(const CLI_CONF conf)
{
    m_tCliConf = conf;
    startSockThd(conf.nNetThd);
}

void NetEngine::connSvr(const QString ip, const quint16 nPort)
{
    emit sign_connSvr(ip, nPort, m_tCliConf.nSockPerThd);
}

void NetEngine::sendData(QByteArray baData)
{
    emit sign_sendData(baData);
}

void NetEngine::abortSock()
{
    emit sign_abortSock();
}

void NetEngine::startSockThd(const quint16 nThd)
{
    if(nThd <= 0 || nThd > LIMIT_THREAD)
    {
        qDebug() << "Socket thread too much";
        return;
    }

    for (int n=0; n<nThd; n++)
    {
        NetSocket *pSock = new NetSocket();
        QThread *pThd = new QThread(this);
        m_lpNetSockThd.append(pSock);
        m_lpSockThd.append(pThd);

        connect(pSock, SIGNAL(sign_readReady(QObject *, QByteArray)), this, SLOT(slot_recvData(QObject *, QByteArray)));
        connect(pSock, SIGNAL(sign_dealloc(QObject *)), this, SLOT(slot_dealloc(QObject *)));
        connect(pSock, SIGNAL(sign_connected(QObject *)), this, SLOT(slot_connected(QObject *)));
        connect(pSock, SIGNAL(sign_writeReady(QObject *)), this, SLOT(slot_writeReady(QObject *)));
        connect(this, SIGNAL(sign_connSvr(QString, quint16, int)), pSock, SLOT(slot_connSvr(QString, quint16, int)));
        connect(this, SIGNAL(sign_abortSock()), pSock, SLOT(slot_abortSock()));
        connect(this, SIGNAL(sign_sendData(QByteArray)), pSock, SLOT(slot_sendData(QByteArray)));

        pThd->start();
        pSock->moveToThread(pThd);
    }
}

void NetEngine::slot_recvData(QObject *pSock, QByteArray baData)
{
    emit sign_output(pSock, baData);
}

void NetEngine::slot_connected(QObject *pSock)
{
    emit sign_count(TP_CONN_ADD);
}

void NetEngine::slot_dealloc(QObject *pSock)
{
    emit sign_count(TP_CONN_DEL);
}

void NetEngine::slot_writeReady(QObject *pSock)
{
    emit sign_count(TP_SEND_ADD);
}
