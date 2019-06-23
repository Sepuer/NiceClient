/*************************************************
ClassName:   LogRecord
Description: 日志
Date:        2019.6
Author:      Sepuer
E-mail:      ucun@qq.com
*************************************************/
#ifndef LOGRECORD_H
#define LOGRECORD_H

#include <QObject>
#include <QMutex>
#include <QDateTime>
#include <QDir>
#include <QTextStream>
#include <QDebug>

#define     DIR_LOG         "./logs/"

class LogRecord : public QObject
{
    Q_OBJECT
public:
    explicit LogRecord(QObject *parent = nullptr);
    static LogRecord *getInstance();
signals:

public slots:
    void slot_startLog();
    void slot_stopLog();

private:
    static LogRecord*   sm_pInstance;
    static QMutex       sm_mutexIns;

public:
    static QFile        sm_logFile;
    static QTextStream  sm_logStream;
    static QMutex       sm_mutexLog;
};

#endif // LOGRECORD_H
