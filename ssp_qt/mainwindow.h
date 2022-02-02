#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <QTcpSocket>
#include <unistd.h>

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
private:
    int SocketInit();
    int SocketRecv();
    int SocketClose();
};
#endif // MAINWINDOW_H
