#include "mainwindow.h"
#include "ui_mainwindow.h"

void cust_delay(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, SLOT(quit()));
    loop.exec();  // 进入一个局部事件循环
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_comm_rev_flag = 0; //初始化flag标志
    m_serial_vol_curr_CH01 = new QSerialPort();
    m_serial_vol_curr_CH01->setPortName(SERIAL_VOL_CURR_CH01_NAME);
    m_serial_vol_curr_CH01->open(QIODevice::ReadWrite);
    m_serial_vol_curr_CH01->setBaudRate(QSerialPort::Baud9600);
    m_serial_vol_curr_CH01->setDataBits(QSerialPort::Data8);
    m_serial_vol_curr_CH01->setParity(QSerialPort::NoParity);
    m_serial_vol_curr_CH01->setStopBits(QSerialPort::OneStop);
    m_serial_vol_curr_CH01->setFlowControl(QSerialPort::NoFlowControl);

    m_serial_vol_curr_CH02 = new QSerialPort();
    m_serial_vol_curr_CH02->setPortName(SERIAL_VOL_CURR_CH02_NAME);
    m_serial_vol_curr_CH02->open(QIODevice::ReadWrite);
    m_serial_vol_curr_CH02->setBaudRate(QSerialPort::Baud9600);
    m_serial_vol_curr_CH02->setDataBits(QSerialPort::Data8);
    m_serial_vol_curr_CH02->setParity(QSerialPort::NoParity);
    m_serial_vol_curr_CH02->setStopBits(QSerialPort::OneStop);
    m_serial_vol_curr_CH02->setFlowControl(QSerialPort::NoFlowControl);

    m_serial_vol_curr_CH03 = new QSerialPort();
    m_serial_vol_curr_CH03->setPortName(SERIAL_VOL_CURR_CH03_NAME);
    m_serial_vol_curr_CH03->open(QIODevice::ReadWrite);
    m_serial_vol_curr_CH03->setBaudRate(QSerialPort::Baud9600);
    m_serial_vol_curr_CH03->setDataBits(QSerialPort::Data8);
    m_serial_vol_curr_CH03->setParity(QSerialPort::NoParity);
    m_serial_vol_curr_CH03->setStopBits(QSerialPort::OneStop);
    m_serial_vol_curr_CH03->setFlowControl(QSerialPort::NoFlowControl);

    m_serial_power_communication = new QSerialPort();
    m_serial_power_communication->setPortName(SERIAL_POWER_COMMUNICATION_NAME);
    m_serial_power_communication->open(QIODevice::ReadWrite);
    m_serial_power_communication->setBaudRate(QSerialPort::Baud115200);
    m_serial_power_communication->setDataBits(QSerialPort::Data8);
    m_serial_power_communication->setParity(QSerialPort::NoParity);
    m_serial_power_communication->setStopBits(QSerialPort::OneStop);
    m_serial_power_communication->setFlowControl(QSerialPort::NoFlowControl);

    connect(m_serial_vol_curr_CH01, SIGNAL(readyRead()), this, SLOT(read_serial_vol_curr_CH01()));
    connect(m_serial_vol_curr_CH02, SIGNAL(readyRead()), this, SLOT(read_serial_vol_curr_CH02()));
    connect(m_serial_vol_curr_CH03, SIGNAL(readyRead()), this, SLOT(read_serial_vol_curr_CH03()));
    connect(m_serial_power_communication, SIGNAL(readyRead()), this, SLOT(read_serial_power_communication()));


    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    m_timer->start(1000);

    m_master_timer = new QTimer(this);
    connect(m_master_timer, SIGNAL(timeout()), this, SLOT(on_master_timeout()));

    m_slave_timer = new QTimer(this);
    connect(m_slave_timer, SIGNAL(timeout()), this, SLOT(on_slave_timeout()));
    m_slave_timer->start(3000);

    // 发送查找ID的命令
//    m_serial_power_communication->writeData((const char *)("AT+ADDR?\r\n"), 10);
    m_serial_power_communication->write("AT+ADDR?\r\n");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parse_vol_cur_data(int index_ch, QString inputs)
{
    QString test1 = inputs.remove(' ');
    QString input = test1.split("\r\n").first();
    // 使用"|"分割字符串并去除空项
    QStringList parts = input.split("|", QString::SkipEmptyParts);
    for(int i = 0; i < parts.size(); i++)
    {
        qDebug()<<parts[i];
    }

    if((m_id == 1) && (index_ch == 0))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_01_ch01_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_01_ch01_cur->setText(cur_list[1]);
    }
    else if((m_id == 1) && (index_ch == 1))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_01_ch02_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_01_ch02_cur->setText(cur_list[1]);
    }
    else if((m_id == 1) && (index_ch == 2))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_01_ch03_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_01_ch03_cur->setText(cur_list[1]);
    }
    else if((m_id == 2) && (index_ch == 0))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_02_ch01_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_02_ch01_cur->setText(cur_list[1]);
    }
    else if((m_id == 2) && (index_ch == 1))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_02_ch02_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_02_ch02_cur->setText(cur_list[1]);
    }
    else if((m_id == 2) && (index_ch == 2))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_02_ch03_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_02_ch03_cur->setText(cur_list[1]);
    }
    else if((m_id == 3) && (index_ch == 0))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_03_ch01_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_03_ch01_cur->setText(cur_list[1]);
    }
    else if((m_id == 3) && (index_ch == 1))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_03_ch02_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_03_ch02_cur->setText(cur_list[1]);
    }
    else if((m_id == 3) && (index_ch == 2))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_03_ch03_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_03_ch03_cur->setText(cur_list[1]);
    }
    else if((m_id == 4) && (index_ch == 0))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_04_ch01_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_04_ch01_cur->setText(cur_list[1]);
    }
    else if((m_id == 4) && (index_ch == 1))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_04_ch02_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_04_ch02_cur->setText(cur_list[1]);
    }
    else if((m_id == 4) && (index_ch == 2))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_04_ch03_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_04_ch03_cur->setText(cur_list[1]);
    }
    else if((m_id == 5) && (index_ch == 0))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_05_ch01_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_05_ch01_cur->setText(cur_list[1]);
    }
    else if((m_id == 5) && (index_ch == 1))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_05_ch02_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_05_ch02_cur->setText(cur_list[1]);
    }
    else if((m_id == 5) && (index_ch == 2))
    {
        QStringList vol_list = parts[0].split(":", QString::SkipEmptyParts);
        ui->salve_05_ch03_vol->setText(vol_list[1]);
        QStringList cur_list = parts[1].split(":", QString::SkipEmptyParts);
        ui->salve_05_ch03_cur->setText(cur_list[1]);
    }

    // 确保有6个部分
//    if (parts.size() != 6) {
//        qWarning() << "Invalid input format, expected 6 parts, got" << parts.size();
//        return;
//    }

    // 定义正则表达式匹配浮点数（包括符号、整数和小数部分）
    QRegularExpression numRegex(R"([-+]?\d+\.\d+)");

    // 存储解析结果的结构
    struct Measurement {
        QString name;
        float value;
        QString unit;
    };

    QVector<Measurement> measurements;

    for (const QString &part : parts) {
        // 去除前后空格
        QString trimmedPart = part.trimmed();

        // 分割名称和值+单位部分
        int colonPos = trimmedPart.indexOf(":");
        if (colonPos == -1) {
            qWarning() << "Invalid part format (missing colon):" << trimmedPart;
            return;
        }

        QString name = trimmedPart.left(colonPos).trimmed();
        QString valueWithUnit = trimmedPart.mid(colonPos + 1).trimmed();

        // 使用正则表达式匹配数值
        QRegularExpressionMatch match = numRegex.match(valueWithUnit);
        if (!match.hasMatch()) {
            qWarning() << "No numeric value found in:" << valueWithUnit;
            return;
        }

        QString valueStr = match.captured();
        int valueEndPos = match.capturedEnd();
        QString unit = valueWithUnit.mid(valueEndPos).trimmed();

        // 转换为浮点数
        bool ok;
        float value = valueStr.toFloat(&ok);
        if (!ok) {
            qWarning() << "Failed to convert to float:" << valueStr;
            return;
        }

        // 处理KW*H特殊情况（转换为标准单位）
        if (name == "W" && unit == "KW*H") {
            value *= 1000; // 转换为瓦时
            unit = "WH";
        }

        measurements.append({name, value, unit});
    }
#if 1
    // 打印解析结果
    for(int i = 0; i < measurements.size(); i++)
    {
        m_CH_value[index_ch][i] = measurements.at(i).value;
    }
    qDebug()<<m_CH_value[index_ch][0]<<" "<<m_CH_value[index_ch][1]<<" "<<m_CH_value[index_ch][2]<<" "<<m_CH_value[index_ch][3]<<" "<<m_CH_value[index_ch][4]<<" "<<m_CH_value[index_ch][5];
#endif
    for (const auto &m : measurements) {
        qDebug() << m.name << ":" << m.value << m.unit;
    }

}
// COMM接收的数据类型有：
// +ADDR=000000000001
// +RX=00,000000000001,0009,123456789
// +RX=01,000000000001,0009,123456789
// +TX=OK
int MainWindow::parse_comm_data(QString str)
{
    bool ok;
    qint64 rev_id = 0;
    qint64 rev_len = 0;
    float vol_cur[6];
    if(str.contains("+ADDR="))
    {
        QStringList parts = str.split("=");
        if(parts.size() >= 2)
        {
            m_ID = parts[1];
            m_id = parts[1].toLongLong(&ok, 16);
            qDebug()<<"m_ID:"<<m_ID;
        }
    }
    else if(str.contains("+RX="))
    {
        QStringList parts = str.split(",");
        if(parts.size() != 4)
        {
            qDebug()<<"error:"<<str;
            return -1;
        }

        QStringList temp = parts[0].split("=");
        if(temp[1].contains("00"))
        {
            // 00-点对点通信，需要回复    01-广播通信，不需要回复
        }

        // 解析设备ID，通过ID确认设备是否已经记录
        if(! m_dev_list.contains(parts[1]))
        {
            m_dev_list.append(parts[1]);
        }
        rev_id = parts[1].toLongLong(&ok, 16);
        if(!ok)
        {
            qDebug()<<"change ID to Hex fail.";
        }

        // 解析设备发送的数据长度
        rev_len = parts[2].toLongLong(&ok, 16);
        if(!ok)
        {
            qDebug()<<"change Len to Hex fail.";
        }

        // 解析设备的数据
        QByteArray rev_data = parts[3].toUtf8();
        const char* charPtr = rev_data.constData();
        if(check_data(charPtr, rev_len) != 0)
        {
            qDebug()<<"rev data check fail.";
            return -1;
        }

        // 收到的数据是常规数据
        if(DATA_IS_DATA == charPtr[1])
        {
            memcpy(&vol_cur, &charPtr[2], 24);
            memcpy(&m_ctl_value, &charPtr[26], 5);
            if(rev_id == 1)
            {
                ui->salve_01_ch01_vol->setText(QLocale().toString(vol_cur[0]) + "V");
                ui->salve_01_ch02_vol->setText(QLocale().toString(vol_cur[2]) + "V");
                ui->salve_01_ch03_vol->setText(QLocale().toString(vol_cur[4]) + "V");
                ui->salve_01_ch01_cur->setText(QLocale().toString(vol_cur[1]) + "A");
                ui->salve_01_ch02_cur->setText(QLocale().toString(vol_cur[3]) + "A");
                ui->salve_01_ch03_cur->setText(QLocale().toString(vol_cur[5]) + "A");
            }
            else if(rev_id == 2)
            {
                ui->salve_02_ch01_vol->setText(QLocale().toString(vol_cur[0]) + "V");
                ui->salve_02_ch02_vol->setText(QLocale().toString(vol_cur[2]) + "V");
                ui->salve_02_ch03_vol->setText(QLocale().toString(vol_cur[4]) + "V");
                ui->salve_02_ch01_cur->setText(QLocale().toString(vol_cur[1]) + "A");
                ui->salve_02_ch02_cur->setText(QLocale().toString(vol_cur[3]) + "A");
                ui->salve_02_ch03_cur->setText(QLocale().toString(vol_cur[5]) + "A");
            }
            else if(rev_id == 3)
            {
                ui->salve_03_ch01_vol->setText(QLocale().toString(vol_cur[0]) + "V");
                ui->salve_03_ch02_vol->setText(QLocale().toString(vol_cur[2]) + "V");
                ui->salve_03_ch03_vol->setText(QLocale().toString(vol_cur[4]) + "V");
                ui->salve_03_ch01_cur->setText(QLocale().toString(vol_cur[1]) + "A");
                ui->salve_03_ch02_cur->setText(QLocale().toString(vol_cur[3]) + "A");
                ui->salve_03_ch03_cur->setText(QLocale().toString(vol_cur[5]) + "A");
            }
            else if(rev_id == 4)
            {
                ui->salve_04_ch01_vol->setText(QLocale().toString(vol_cur[0]) + "V");
                ui->salve_04_ch02_vol->setText(QLocale().toString(vol_cur[2]) + "V");
                ui->salve_04_ch03_vol->setText(QLocale().toString(vol_cur[4]) + "V");
                ui->salve_04_ch01_cur->setText(QLocale().toString(vol_cur[1]) + "A");
                ui->salve_04_ch02_cur->setText(QLocale().toString(vol_cur[3]) + "A");
                ui->salve_04_ch03_cur->setText(QLocale().toString(vol_cur[5]) + "A");
            }
            else if(rev_id == 5)
            {
                ui->salve_05_ch01_vol->setText(QLocale().toString(vol_cur[0]) + "V");
                ui->salve_05_ch02_vol->setText(QLocale().toString(vol_cur[2]) + "V");
                ui->salve_05_ch03_vol->setText(QLocale().toString(vol_cur[4]) + "V");
                ui->salve_05_ch01_cur->setText(QLocale().toString(vol_cur[1]) + "A");
                ui->salve_05_ch02_cur->setText(QLocale().toString(vol_cur[3]) + "A");
                ui->salve_05_ch03_cur->setText(QLocale().toString(vol_cur[5]) + "A");
            }
        }
        else if(DATA_IS_MASTER == charPtr[1])
        {
            // 收到竞选master的消息
            qDebug()<<rev_id<<" campaign";
        }

    }

    return 0;
}

int MainWindow::check_data(const char *data, int len)
{
    char value = 0;
    for(int i = 0; i < len-2; i++)
    {
        value += data[i];
    }

    if(value == data[len-1])
    {
        return 0;
    }
    return -1;
}


void MainWindow::read_serial_vol_curr_CH01()
{
    QByteArray read_buff;
    cust_delay(1000);
    read_buff = m_serial_vol_curr_CH01->readAll();
    qDebug("CH01 read:%s\n", qPrintable(QString(read_buff)));
    parse_vol_cur_data(0, QString(read_buff));

}

void MainWindow::read_serial_vol_curr_CH02()
{
    QByteArray read_buff;
    cust_delay(1000);
    read_buff = m_serial_vol_curr_CH02->readAll();
    qDebug("CH02 read:%s\n", qPrintable(QString(read_buff)));
    parse_vol_cur_data(1, QString(read_buff));
}

void MainWindow::read_serial_vol_curr_CH03()
{
    QByteArray read_buff;
    cust_delay(1000);
    read_buff = m_serial_vol_curr_CH03->readAll();
    qDebug("CH03 read:%s\n", qPrintable(QString(read_buff)));
    parse_vol_cur_data(2, QString(read_buff));
}

void MainWindow::read_serial_power_communication()
{
    QByteArray read_buff;
    cust_delay(120);
    m_comm_rev_flag = 1;        // 电力载波通信有收到数据
    read_buff = m_serial_power_communication->readAll();
    qDebug("COMM read:%s\n", qPrintable(QString(read_buff)));
}

void MainWindow::on_timeout()
{
    qDebug()<<"this is timeout";

    m_timer->stop();
    m_serial_vol_curr_CH01->write(">>GetVal");
    m_serial_vol_curr_CH02->write(">>GetVal");
    m_serial_vol_curr_CH03->write(">>GetVal");
    m_timer->start(2000);

}

void MainWindow::on_master_timeout()
{

}

void MainWindow::on_slave_timeout()
{
    if(m_comm_rev_flag == 0)
    {
        m_master_timer->start(1000);
    }
}

void MainWindow::on_salve_01_ctr01_clicked()
{

}

void MainWindow::on_salve_01_ctr02_clicked()
{

}

void MainWindow::on_salve_01_ctr03_clicked()
{

}

void MainWindow::on_salve_01_ctr04_clicked()
{

}

void MainWindow::on_salve_01_ctr05_clicked()
{

}

void MainWindow::on_salve_01_ctr06_clicked()
{

}

void MainWindow::on_salve_02_ctr01_clicked()
{

}

void MainWindow::on_salve_02_ctr02_clicked()
{

}

void MainWindow::on_salve_02_ctr03_clicked()
{

}

void MainWindow::on_salve_02_ctr04_clicked()
{

}

void MainWindow::on_salve_02_ctr05_clicked()
{

}

void MainWindow::on_salve_02_ctr06_clicked()
{

}

void MainWindow::on_salve_03_ctr01_clicked()
{

}

void MainWindow::on_salve_03_ctr02_clicked()
{

}

void MainWindow::on_salve_03_ctr03_clicked()
{

}

void MainWindow::on_salve_03_ctr04_clicked()
{

}

void MainWindow::on_salve_03_ctr05_clicked()
{

}

void MainWindow::on_salve_03_ctr06_clicked()
{

}

void MainWindow::on_salve_04_ctr01_clicked()
{

}

void MainWindow::on_salve_04_ctr02_clicked()
{

}

void MainWindow::on_salve_04_ctr03_clicked()
{

}

void MainWindow::on_salve_04_ctr04_clicked()
{

}

void MainWindow::on_salve_04_ctr05_clicked()
{

}

void MainWindow::on_salve_04_ctr06_clicked()
{

}

void MainWindow::on_salve_05_ctr01_clicked()
{

}

void MainWindow::on_salve_05_ctr02_clicked()
{

}

void MainWindow::on_salve_05_ctr03_clicked()
{

}

void MainWindow::on_salve_05_ctr04_clicked()
{

}

void MainWindow::on_salve_05_ctr05_clicked()
{

}

void MainWindow::on_salve_05_ctr06_clicked()
{

}
