#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    is_logined = false;
    // ui->tblMessage->setRowCount(12);
    ui->tblMessage->setColumnCount(3);
    ui->tblMessage->setColumnWidth(1, 100);
    ui->tblMessage->setColumnWidth(2, 100);
    ui->tblMessage->setColumnWidth(3, 300);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnLogin_clicked()
{
    int ret = SocketInit();
    if (ret == 0) {
        is_logined = true;
        loginReq.set_version(1);
        loginReq.set_mess_type(LOGIN_REQ);
        loginReq.set_user_name("hank");
        loginReq.set_password("123456");
        /**
        * 以send_buffer + 3为开始,将后面的10240个字符序列化
        */
        loginReq.SerializeToArray(send_buffer + 3, 10240);
        send_buffer[0] = '1';
        send_buffer[1] = '0';
        send_buffer[2] = '1';
        /**
        * 传输send_buffer字符串,传输大小为loginReq.ByteSize() + 3个
        */
        ::send(sock_client, send_buffer, loginReq.ByteSize() + 3, 0);
        SocketRecv();
    }
}

void MainWindow::on_btnLogout_clicked()
{
    ::close(sock_client);
    is_logined = false;
}

void MainWindow::on_btnFresh_clicked()
{
    if (!is_logined) {
        QMessageBox msgBox;
        msgBox.setText("please login");
        msgBox.exec();
    }
    getMessageReq.set_verion(1);
    getMessageReq.set_mess_type(GET_MESSAGE_REQ); // 205
    getMessageReq.set_user_id(user_id);
    pulishReq.SerializeToArray(send_buffer + 3, 10240);
    send_buffer[0] = '2';
    send_buffer[1] = '0';
    send_buffer[2] = '5';
    ::send(sock_client, send_buffer, loginReq.ByteSize() + 3, 0);
    SocketRecv();
}

void MainWindow::on_btnPushlish_clicked()
{
    if (!is_logined) {
        QMessageBox msgBox;
        msgBox.setText("please login");
        msgBox.exec();
    }
    /** 数据转换
     * Qstring -> QByteArray -> char *
     */
    QString qs = ui->edtMessage->toPlainText();
    QByteArray qba = qs.toUtf8();
    char *msg = qba.data();

    pulishReq.set_verion(1);
    pulishReq.set_mess_type(PUBLISH_MESSAGE_LIST_REQ);
    pulishReq.set_user_id(user_id);
    pulishReq.set_content(msg);
    pulishReq.SerializeToArray(send_buffer + 3, 10240);
    send_buffer[0] = '2';
    send_buffer[1] = '0';
    send_buffer[2] = '1';
    ::send(sock_client, send_buffer, loginReq.ByteSize() + 3, 0);
    SocketRecv();
}

int MainWindow::SocketInit() {
    sock_client = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    memset(server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(8999);
    int conn_ret = ::connect(sock_client, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if (conn_ret < 0) {
        return -1;
    }

    return 0;
}
int MainWindow::SocketRecv() {
    int ret = ::recv(sock_client, recv_buffer, 10240, 0);
    if (ret > 0) {
        RecvRep();
    }
    return -1;
}
int MainWindow::SocketClose() {
    ::close(sock_client);
    return -1;
}

int MainWindow::RecvRep()
{
    QMessageBox msgBox;
    msgBox.setText("收到回包");
    msgBox.exec();
    // todo
    int mess_type = (recv_buffer[0] - '0') * 100 + (recv_buffer[1] - '0') * 10 + (recv_buffer[2] - '0') * 1;
    switch (mess_type) {
        case LOGIN_REP: // login_rsp
            memset(recv_buffer, 0, 10240);
            loginRep.ParseFromArray(recv_buffer + 3, 10240);
            if (loginRep.ret() == 0) {
                user_id = loginRep.user_id();
            }
            break;
        case PUBLISH_MESSAGE_REP:
            pulishReq.ParseFromArray(recv_buffer + 3, 10240);
            memset(recv_buffer, 0, 10240);
            break;
        case GET_MESSAGE_LIST_REP:
            getMessageReq.ParseFromArray(recv_buffer + 3, 10240);
            memset(recv_buffer, 0, 10240);
            if (getMessageReq.ret() == 0) {
                ui->tblMessage->setRowCount(12); // 设置行
                for (int i = 0; i < getMessageReq.message_list_size(); i ++) {
                    int mess_id = getMessageReq.message_list().message_id();
                    // getMessageReq.message_list(i).content();
                    char mess_id_str[256];
                    sprintf(mess_id_str, "%d", mess_id);
                    ui->tblMessage->setItem(i, 0, new QTableWidgetItem(mess_id_str));
                    ui->tblMessage->setItem(i, 1, new QTableWidgetItem(getMessageReq.message_list(i).publisher_name().c_str()));
                    ui->tblMessage->setItem(i, 2, new QTableWidgetItem(getMessageReq.message_list(i).content().c_str()));
                }
            }
            break;
        default:
            break;
    }
    return 0;
}
