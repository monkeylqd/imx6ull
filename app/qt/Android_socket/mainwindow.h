#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>
#include <QLabel>
#include <QMutex>

#define APP_ID  0xa0

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int button_click(int ch, int index);    // CTL按钮点击后，m_ctl_value值的改变
    int send_socket_data();                 // 发送socket数据。当点击按钮，CTL状态发生改变时，就发送数据。
    int up_ui(unsigned char *cdata);                            // 更新VOL、CUR、CTL等值

public:
    QList<QPushButton*> m_buttonList;       // 保存所有的CTL button按钮
    QList<QLabel*> m_vol_cur_LabelList;     // 保存所有的电压、电流label值
    QList<QLabel*> m_slave_name_LabelList;     // 保存所有的电压、电流label值
    unsigned char m_ctl_value[5];           // 保存5个分机的CTL值
    unsigned int m_vol[5][3];               // 保存5个分机3个通道的电压值
    unsigned int m_cur[5][3];               // 保存5个分机3个通道的电流值
    unsigned char m_send_buff[9];           // 用于组织socket发送的数据内存
    QMutex m_mutex;                         // 用于发送socket数据的时候上锁

    unsigned char total_buffer[4096];       // 接收到的数据保存到这里，用于后续解析
    int w_index = 0;                        // total_buffer里面有多少数据

private slots:
    void on_connect_button_clicked();       // 连接socket按钮

    void connected_slot();
    void disconnect_slot();
    void readyRead_slot();


    void on_slave_1_ctl1_clicked();

    void on_slave_1_ctl2_clicked();

    void on_slave_1_ctl3_clicked();

    void on_slave_1_ctl4_clicked();

    void on_slave_1_ctl5_clicked();

    void on_slave_1_ctl6_clicked();

    void on_slave_2_ctl1_clicked();

    void on_slave_2_ctl2_clicked();

    void on_slave_2_ctl3_clicked();

    void on_slave_2_ctl4_clicked();

    void on_slave_2_ctl5_clicked();

    void on_slave_2_ctl6_clicked();

    void on_slave_3_ctl1_clicked();

    void on_slave_3_ctl2_clicked();

    void on_slave_3_ctl3_clicked();

    void on_slave_3_ctl4_clicked();

    void on_slave_3_ctl5_clicked();

    void on_slave_3_ctl6_clicked();

    void on_slave_4_ctl1_clicked();

    void on_slave_4_ctl2_clicked();

    void on_slave_4_ctl3_clicked();

    void on_slave_4_ctl4_clicked();

    void on_slave_4_ctl5_clicked();

    void on_slave_4_ctl6_clicked();

    void on_slave_5_ctl1_clicked();

    void on_slave_5_ctl2_clicked();

    void on_slave_5_ctl3_clicked();

    void on_slave_5_ctl4_clicked();

    void on_slave_5_ctl5_clicked();

    void on_slave_5_ctl6_clicked();

private:
    Ui::MainWindow *ui;
    QTcpSocket *m_tcpsocket;
    int m_connect_flag;
};
#endif // MAINWINDOW_H
