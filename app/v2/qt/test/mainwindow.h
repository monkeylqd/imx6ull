#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>
#include <QMutex>
#include <QLabel>

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
    void on_pushButton_3_clicked();
    void connected_slot();
    void disconnect_slot();
    void readyRead_slot();

    void on_pushButton_1_A_clicked();
    void on_pushButton_1_B_clicked();
    void on_pushButton_1_C_clicked();

    void on_pushButton_2_A_clicked();
    void on_pushButton_2_B_clicked();
    void on_pushButton_2_C_clicked();

    void on_pushButton_3_A_clicked();
    void on_pushButton_3_B_clicked();
    void on_pushButton_3_C_clicked();

    void on_pushButton_4_A_clicked();
    void on_pushButton_4_B_clicked();
    void on_pushButton_4_C_clicked();

    void on_pushButton_5_A_clicked();
    void on_pushButton_5_B_clicked();
    void on_pushButton_5_C_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *m_tcpsocket;
    QList<QPushButton*> m_buttonList;
    QList<QLabel*> m_vol_cur_LabelList;     // 保存所有的电压、电流label值
    int m_connect_flag;
    unsigned char sub_status[2][6]; // sub_status[0][i]表示状态  sub_status[1][i]表示颜色
    int set_sub_status();
    int update_ui(unsigned char *data, int len);
    unsigned char m_send_buff[8];           // 用于组织socket发送的数据内存
    QMutex m_mutex;                         // 用于发送socket数据的时候上锁
    unsigned char total_buff[4096];       // 接收到的数据保存到这里，用于后续解析
    int m_total_len;
};
#endif // MAINWINDOW_H
