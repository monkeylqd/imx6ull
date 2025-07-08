#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QTimer>
#include <QDebug>

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
    QTimer *m_timer;
#if USE_DEBUG
    QSerialPort *m_serial;
#else
    float m_CH_value[3][6];

    unsigned long long int m_ID;                // 设备ID，8个字节
    int m_device_type;                          // 设备类型，主机还是从机
    int m_comm_rev_flag;                        // 电力载波通信接口是否有收到数据的flag
    QSerialPort *m_serial_vol_curr_CH01;        // 采集电压、电流的第1路串口
    QSerialPort *m_serial_vol_curr_CH02;        // 采集电压、电流的第2路串口
    QSerialPort *m_serial_vol_curr_CH03;        // 采集电压、电流的第3路串口
    QSerialPort *m_serial_power_communication;  // 电力载波通信串口
#endif


public:
#if USE_DEBUG
#else
    int parse_uart_data();
    void parseMeasurementData(int index_salve, QString inputs);     // 解析电压、电流的串口数据
#endif
private slots:
#if USE_DEBUG
    void read_serial_data();
    void on_pushButton_clicked();
#else
    void read_serial_vol_curr_CH01();
    void read_serial_vol_curr_CH02();
    void read_serial_vol_curr_CH03();
    void read_serial_power_communication();
#endif

    void on_timeout();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
