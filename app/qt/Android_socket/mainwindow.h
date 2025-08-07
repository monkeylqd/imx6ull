#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTextEdit>
#include <QPushButton>
#include <QTimer>
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
    int button_click(int ch, int index);

public:
    QList<QPushButton*> m_buttonList;
    QList<QLabel*> m_vol_cur_LabelList;
    unsigned char m_ctl_value[5];
    unsigned int m_vol[5][3];
    unsigned int m_cur[5][3];
private slots:
    void on_connect_button_clicked();

    void on_send_button_clicked();
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
