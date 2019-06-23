#ifndef DEF_H
#define DEF_H

#include <QObject>

#define     FILE_CONFIG         "NiceClient.ini"
#define     DIR_CMD             "cmd"
#define     LIMIT_THREAD        1000        //自定义

#define     TP_CONN_ADD         0
#define     TP_CONN_DEL         1
#define     TP_RECV_ADD         2
#define     TP_SEND_ADD         3

typedef struct tagCLI_CONF
{
   bool     bLog;               //是否启用日志
   quint16  nNetThd;            //通信线程数
   quint16  nPort;              //端口号
   quint16  nSendTimer;         //发送间隔时间
   int      nSockPerThd;        //每个线程连接数
   QString  addrSvr;            //服务器地址
}CLI_CONF;

#endif // DEF_H
