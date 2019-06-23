#include "LogRecord.h"
LogRecord* LogRecord::sm_pInstance = nullptr;
QMutex LogRecord::sm_mutexIns;
QMutex LogRecord::sm_mutexLog;
QFile LogRecord::sm_logFile;
QTextStream LogRecord::sm_logStream;

void reLog(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    LogRecord::sm_mutexLog.lock();
    QString log;
    QByteArray baMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s (%s:%u, %s)\n", baMsg.constData(), context.file, context.line, context.function);
        break;
    case QtInfoMsg:
        fprintf(stderr, "Info: %s (%s:%u, %s)\n", baMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s (%s:%u, %s)\n", baMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s (%s:%u, %s)\n", baMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", baMsg.constData(), context.file, context.line, context.function);
        abort();
    }

    //QString codePath = QString("(%1, %2)").arg(context.file).arg(context.line);
    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //log += QString("%1 %2 %3").arg(msg, codePath, dateTime);
    log += QString("%1 %3").arg(msg, dateTime);

    QString logDir = QString("%1%2").arg(DIR_LOG, QDate::currentDate().toString("yyyyMMdd"));

    QDir dirLog;
    if(!dirLog.exists(logDir))
    {
        if(!dirLog.mkdir(logDir))
        {
            qDebug("log dir create failed!");
            return;
        }
    }
    QString fileName = QString("%1/%2.log").arg(logDir).arg(QDateTime::currentDateTime().toString("yyyyMMddhh"));

    if(LogRecord::sm_logFile.fileName() != fileName)
    {
        if(LogRecord::sm_logFile.isOpen())
        {
            LogRecord::sm_logFile.close();
        }

        LogRecord::sm_logFile.setFileName(fileName);
        bool bOpen = LogRecord::sm_logFile.open(QIODevice::WriteOnly | QIODevice::Append | QFile::Text);
        if(!bOpen)
        {
            return;
        }
    }

    LogRecord::sm_logStream.setDevice(&LogRecord::sm_logFile);
    LogRecord::sm_logStream << log << "\n";
    LogRecord::sm_logStream.flush();

    LogRecord::sm_mutexLog.unlock();
}

LogRecord::LogRecord(QObject *parent) : QObject(parent)
{

}

LogRecord *LogRecord::getInstance()
{
    if(sm_pInstance == nullptr)
    {
        QMutexLocker locker(&sm_mutexIns);
        if(sm_pInstance == nullptr)
        {
            sm_pInstance = new LogRecord;
        }
    }
    return sm_pInstance;
}

void LogRecord::slot_startLog()
{
    qInstallMessageHandler(reLog);
    QDir dirLog;
    if(!dirLog.exists(DIR_LOG))
    {
        if(!dirLog.mkdir(DIR_LOG))
        {
            qDebug("logs dir create failed!");
            return;
        }
    }
}

void LogRecord::slot_stopLog()
{
    qInstallMessageHandler(nullptr);
}

