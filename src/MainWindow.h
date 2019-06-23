#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QThread>
#include <QStringListModel>
#include <QTimer>
#include "Def.h"
#include "LogRecord.h"
#include "NetEngine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //读取配置
    void initConf();
    //读取指令文件
    void initCmd();
    //启用日志
    void runLog();
    //初始化客户端
    void runCli();

signals:
    void sign_startLog();
    void sign_stopLog();

private slots:
    void slot_output(QObject *pSock, QByteArray baData);
    void slot_count(int nType);
    void on_pbConnect_clicked();

    void on_lvCom_clicked(const QModelIndex &index);

    void on_pbSend_clicked();

    void on_pbAutoSend_clicked();

    void on_lvCom_doubleClicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;

    CLI_CONF    m_tCliConf;         //客户端配置
    LogRecord*  m_pLog;
    QThread*    m_pLogThd;
    NetEngine   m_cliEngine;

    int         m_nConnSum;
    int         m_nOnline;
    int         m_nSendSum;
    int         m_nRecvSum;

    QMap<QString, QString>      m_mapCmd;
    QTimer*     m_pTimerSend;
    bool        m_bConnClick;
};

#endif // MAINWINDOW_H
