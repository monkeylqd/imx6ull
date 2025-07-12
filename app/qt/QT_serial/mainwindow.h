#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QTimer>
#include <QDebug>
#include <QProcess>

#include <QStringList>
#include <QRegularExpression>

#define USE_DEBUG    0

#define SERIAL_1_NAME   ("/dev/ttyUSB0")
#define SERIAL_VOL_CURR_CH01_NAME           ("/dev/ttymxc1")
#define SERIAL_VOL_CURR_CH02_NAME           ("/dev/ttymxc2")
#define SERIAL_VOL_CURR_CH03_NAME           ("/dev/ttymxc4")
#define SERIAL_POWER_COMMUNICATION_NAME     ("/dev/ttymxc5")

enum DEVICES_TYPE
{
    DEVICES_TYPE_MASTER=0,
    DEVICES_TYPE_SALVE=1,
};

enum REV_DATA_TYPE
{
    DATA_IS_MASTER = 1,
    DATA_IS_DATA = 2,
};

enum ENUM_TYPE
{
    ENUM_TYPE_OUT = 0,
    ENUM_TYPE_IN = 1,
};

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    QList<QPushButton*> m_buttonList;
    QList<QLabel*> m_vol_cur_LabelList;
    QList<QLabel*> m_salve_name_LabelList;
    QTimer *m_timer;
    QTimer *m_master_timer;
    QTimer *m_slave_timer;
    QTimer *m_comm_send_timer;
    QStringList m_ctl_cmd;

    int m_CH_value[3][6];       // 保持3路通道的电流电压值
    int m_ctl_value[5];        // 保存5个分机的开关控制信息，按bit去控制
    int m_click_flag[5];

    QString m_ID;                // 设备ID，8个字节
    qint64 m_id;
    QList<QString> m_dev_list;
    int m_device_type;                          // 设备类型，主机还是从机
    int m_comm_rev_flag;                        // 电力载波通信接口是否有收到数据的flag
    QSerialPort *m_serial_vol_curr_CH01;        // 采集电压、电流的第1路串口
    QSerialPort *m_serial_vol_curr_CH02;        // 采集电压、电流的第2路串口
    QSerialPort *m_serial_vol_curr_CH03;        // 采集电压、电流的第3路串口
    QSerialPort *m_serial_power_communication;  // 电力载波通信串口



public:
    int parse_uart_data();
    void parse_vol_cur_data(int index_salve, QString inputs);     // 解析电压、电流的串口数据
    int parse_comm_data(QString str);
    int check_data(const char *data, int len);
    int run_ctl_cmd(int dir, int index, int value);
    int up_ctl_status(void);

private slots:

    void read_serial_vol_curr_CH01();
    void read_serial_vol_curr_CH02();
    void read_serial_vol_curr_CH03();
    void read_serial_power_communication();


    void on_timeout();
    void on_comm_send_timeout();
    void on_master_timeout();
    void on_slave_timeout();

    void on_salve_01_ctr01_clicked();

    void on_salve_01_ctr02_clicked();

    void on_salve_01_ctr03_clicked();

    void on_salve_01_ctr04_clicked();

    void on_salve_01_ctr05_clicked();

    void on_salve_01_ctr06_clicked();

    void on_salve_02_ctr01_clicked();

    void on_salve_02_ctr02_clicked();

    void on_salve_02_ctr03_clicked();

    void on_salve_02_ctr04_clicked();

    void on_salve_02_ctr05_clicked();

    void on_salve_02_ctr06_clicked();

    void on_salve_03_ctr01_clicked();

    void on_salve_03_ctr02_clicked();

    void on_salve_03_ctr03_clicked();

    void on_salve_03_ctr04_clicked();

    void on_salve_03_ctr05_clicked();

    void on_salve_03_ctr06_clicked();

    void on_salve_04_ctr01_clicked();

    void on_salve_04_ctr02_clicked();

    void on_salve_04_ctr03_clicked();

    void on_salve_04_ctr04_clicked();

    void on_salve_04_ctr05_clicked();

    void on_salve_04_ctr06_clicked();

    void on_salve_05_ctr01_clicked();

    void on_salve_05_ctr02_clicked();

    void on_salve_05_ctr03_clicked();

    void on_salve_05_ctr04_clicked();

    void on_salve_05_ctr05_clicked();

    void on_salve_05_ctr06_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
