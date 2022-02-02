#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
//#include "" 此处要包含mess_define.proto
#include "/home/liuhao/ssp/common/mess_type.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_btnLogout_clicked();

    void on_btnLogin_clicked();

    void on_btnFresh_clicked();

    void on_btnPushlish_clicked();

private:
    Ui::MainWindow *ui;
private:
    int sock_client;
    bool is_logined;
    int user_id;
private:
    int SocketInit();
    int SocketRecv();
    int SocketClose();
    int RecvRep();
    char recv_buffer[10240];
    char send_buffer[10240];
    ssp::LoginReq loginReq;
    ssp::LoginRep loginRep;
    ssp::LogoutReq logoutReq;
    ssp::LogoutRep logoutRep;
    ssp::PublishMessageReq publishMessageReq;
    ssp::PublishMessageRep publishMessageRep;
    ssp::GetMessageReq getMessageReq;
    ssp::GetMessageRep getMessageRep;
};
#endif // MAINWINDOW_H
