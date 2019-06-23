#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pLog = nullptr;
    m_pLogThd = nullptr;
    m_pTimerSend = nullptr;
    m_bConnClick = false;

    m_nConnSum = 0;
    m_nOnline = 0;
    m_nSendSum = 0;
    m_nRecvSum = 0;

    connect(&m_cliEngine, &NetEngine::sign_output, this, &MainWindow::slot_output);
    connect(&m_cliEngine, &NetEngine::sign_count, this, &MainWindow::slot_count);

    //设置最大显示行数
    ui->pteOutput->document()->setMaximumBlockCount(100);

    initConf();
    initCmd();
    runLog();
    runCli();
}

MainWindow::~MainWindow()
{
    if(m_pLog != nullptr)
    {
        delete m_pLog;
        m_pLog = nullptr;
    }
    if(m_pLogThd != nullptr)
    {
        m_pLogThd->quit();
        m_pLogThd->wait();
        m_pLogThd->deleteLater();
        m_pLogThd = nullptr;
    }
    delete ui;
}

void MainWindow::initConf()
{
    QSettings setSvr(FILE_CONFIG, QSettings::IniFormat);

    m_tCliConf.addrSvr = setSvr.value("Net/AddrSvr", "127.0.0.1").toString();
    m_tCliConf.nPort = setSvr.value("Net/Port", 12345).toInt();
    m_tCliConf.nNetThd = setSvr.value("Net/NetThd", 1).toInt();
    m_tCliConf.nSockPerThd = setSvr.value("Net/SockPerThd", 1).toInt();
    m_tCliConf.bLog = setSvr.value("Sys/Log", 0).toBool();
    m_tCliConf.nSendTimer = setSvr.value("Sys/SendTimer", 1000).toInt();

    ui->leIp->setText(m_tCliConf.addrSvr);
    ui->lePort->setText(QString::number(m_tCliConf.nPort));

    QString status;
    status.sprintf("IP: %s  PORT: %d  CLINET: %d", m_tCliConf.addrSvr.toStdString().data(), m_tCliConf.nPort, m_tCliConf.nNetThd*m_tCliConf.nSockPerThd);
    statusBar()->showMessage(status);
}

void MainWindow::initCmd()
{
    QDir dirCmd(DIR_CMD);
    if(!dirCmd.exists())
    {
        qDebug() << "Cmd dir is not exists!";
        return;
    }
    dirCmd.setFilter(QDir::Files);
    dirCmd.setSorting(QDir::Time);

    QFile fileCmd;
    QFileInfoList lFile = dirCmd.entryInfoList();
    for (int i=0; i<lFile.count(); i++)
    {
        fileCmd.setFileName(lFile.at(i).absoluteFilePath());
        if(fileCmd.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            m_mapCmd[lFile.at(i).baseName()] = QString::fromUtf8(fileCmd.readAll());
        }
        fileCmd.close();
    }

    if(!m_mapCmd.isEmpty())
    {
        QStringListModel *pModel = new QStringListModel(m_mapCmd.keys(), this);
        ui->lvCom->setModel(pModel);
    }
}

void MainWindow::runLog()
{
    if(!m_tCliConf.bLog)
    {
        return;
    }
    m_pLog = LogRecord::getInstance();
    m_pLogThd = new QThread(this);
    connect(this, SIGNAL(sign_startLog()), m_pLog, SLOT(slot_startLog()));
    connect(this, SIGNAL(sign_stopLog()), m_pLog, SLOT(slot_stopLog()));

    m_pLogThd->start();
    m_pLog->moveToThread(m_pLogThd);

    emit sign_startLog();
}

void MainWindow::runCli()
{
    m_cliEngine.cliStart(m_tCliConf);
    m_pTimerSend = new QTimer(this);
    connect(m_pTimerSend, SIGNAL(timeout()), this, SLOT(on_pbSend_clicked()));
    m_pTimerSend->setInterval(m_tCliConf.nSendTimer);
}

void MainWindow::slot_output(QObject *pSock, QByteArray baData)
{
    QString output;
    output.sprintf("[%p]: %s", pSock, baData.toStdString().data());
    ui->pteOutput->appendPlainText(output);
    //qDebug() << output;
    slot_count(TP_RECV_ADD);
}

void MainWindow::slot_count(int nType)
{
    switch (nType)
    {
    case TP_CONN_ADD:
        m_nConnSum++;
        m_nOnline++;
        ui->lbConnSum->setNum(m_nConnSum);
        ui->lbOnline->setNum(m_nOnline);
        break;
    case TP_CONN_DEL:
        m_nOnline--;
        ui->lbOnline->setNum(m_nOnline);
        break;
    case TP_RECV_ADD:
        m_nRecvSum++;
        ui->lbRecvSum->setNum(m_nRecvSum);
        break;
    case TP_SEND_ADD:
        m_nSendSum++;
        ui->lbSendSum->setNum(m_nSendSum);
        break;
    }
}

void MainWindow::on_pbConnect_clicked()
{
    if(!m_bConnClick)
    {
        m_cliEngine.connSvr(ui->leIp->text(), ui->lePort->text().toUShort());
        ui->pbConnect->setText(QStringLiteral("断开"));
        m_bConnClick = true;
    }
    else
    {
        m_cliEngine.abortSock();
        ui->pbConnect->setText(QStringLiteral("连接"));
        m_bConnClick = false;
    }

}

void MainWindow::on_lvCom_clicked(const QModelIndex &index)
{
    ui->pteInput->setPlainText(m_mapCmd[index.data().toString()]);
}

void MainWindow::on_pbSend_clicked()
{
    m_cliEngine.sendData(ui->pteInput->toPlainText().toUtf8());
}

void MainWindow::on_pbAutoSend_clicked()
{
    if(m_pTimerSend->isActive())
    {
        m_pTimerSend->stop();
        ui->pbAutoSend->setText(QStringLiteral("自动发送"));
    }
    else
    {
        m_pTimerSend->start();
        ui->pbAutoSend->setText(QStringLiteral("停止发送"));
    }
}

void MainWindow::on_lvCom_doubleClicked(const QModelIndex &index)
{
    m_cliEngine.sendData(m_mapCmd[index.data().toString()].toUtf8());
}
