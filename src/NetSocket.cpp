#include "NetSocket.h"

NetSocket::NetSocket(QObject *parent) : QObject(parent)
{

}

void NetSocket::slot_connSvr(const QString ip, const quint16 nPort, const int nNub)
{
    if(nNub <= 0)
    {
        return;
    }
    QMutexLocker locker(&m_mutexSock);
    for (int n=0; n<nNub; n++)
    {
        QTcpSocket *pSock = new QTcpSocket(this);
        m_lSock.append(pSock);

        connect(pSock, SIGNAL(connected()), this, SLOT(slot_connected()));
        connect(pSock, SIGNAL(disconnected()), this, SLOT(slot_disconnected()));
        connect(pSock, SIGNAL(readyRead()), this, SLOT(slot_readReady()));

        pSock->connectToHost(ip, nPort);
        if(pSock->waitForConnected())
        {
            //qDebug() << "Connected";
        }
        else
        {
            qDebug("\"[%p]: Connect fail\"", pSock);
        }

    }
}

void NetSocket::slot_connected()
{
    QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
    emit sign_connected(pSock);
    qDebug("\"[%p]: connected\"", pSock);
}

void NetSocket::slot_disconnected()
{
    QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
    emit sign_dealloc(pSock);
    qDebug("\"[%p]: disconnected\"", pSock);
}

void NetSocket::slot_readReady()
{
    QTcpSocket * pSock = qobject_cast<QTcpSocket*>(sender());
    if(pSock)
    {
        QByteArray baRecv = pSock->readAll();
        emit sign_readReady(pSock, baRecv);
    }
}

void NetSocket::slot_sendData(QByteArray baData)
{
    QMutexLocker locker(&m_mutexSock);
    for (int i=0; i<m_lSock.size(); i++)
    {
        m_lSock[i]->write(baData);
        if(m_lSock[i]->waitForBytesWritten())
        {
            emit sign_writeReady(m_lSock[i]);
        }
    }
}

void NetSocket::slot_abortSock()
{
    QMutexLocker locker(&m_mutexSock);
    for (int i=0; i<m_lSock.size(); i++)
    {
        QTcpSocket * pTcpSock = qobject_cast<QTcpSocket*>(m_lSock[i]);
        pTcpSock->abort();
    }
}
