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
//    m_id = 0;
    memset(&m_ctl_value, 0, sizeof(m_ctl_value));
    memset(&m_CH_value, 0, sizeof(m_CH_value));
    memset(&m_click_flag, 0, sizeof(m_click_flag));

    m_ctl_cmd.append("echo 1 > /sys/class/gpio/gpio3/value");
    m_ctl_cmd.append("echo 1 > /sys/class/gpio/gpio115/value");
    m_ctl_cmd.append("echo 1 > /sys/class/gpio/gpio26/value");
    m_ctl_cmd.append("echo 1 > /sys/class/gpio/gpio27/value");
    m_ctl_cmd.append("echo 1 > /sys/class/gpio/gpio22/value");
    m_ctl_cmd.append("echo 1 > /sys/class/gpio/gpio23/value");

    m_ctl_cmd.append("echo 0 > /sys/class/gpio/gpio3/value");
    m_ctl_cmd.append("echo 0 > /sys/class/gpio/gpio115/value");
    m_ctl_cmd.append("echo 0 > /sys/class/gpio/gpio26/value");
    m_ctl_cmd.append("echo 0 > /sys/class/gpio/gpio27/value");
    m_ctl_cmd.append("echo 0 > /sys/class/gpio/gpio22/value");
    m_ctl_cmd.append("echo 0 > /sys/class/gpio/gpio23/value");


    m_ctl_cmd.append("cat /sys/class/gpio/gpio123/value");
    m_ctl_cmd.append("cat /sys/class/gpio/gpio124/value");
    m_ctl_cmd.append("cat /sys/class/gpio/gpio121/value");
    m_ctl_cmd.append("cat /sys/class/gpio/gpio119/value");
    m_ctl_cmd.append("cat /sys/class/gpio/gpio120/value");
    m_ctl_cmd.append("cat /sys/class/gpio/gpio116/value");

    m_buttonList.append(ui->salve_01_ctr01);
    m_buttonList.append(ui->salve_01_ctr02);
    m_buttonList.append(ui->salve_01_ctr03);
    m_buttonList.append(ui->salve_01_ctr04);
    m_buttonList.append(ui->salve_01_ctr05);
    m_buttonList.append(ui->salve_01_ctr06);

    m_buttonList.append(ui->salve_02_ctr01);
    m_buttonList.append(ui->salve_02_ctr02);
    m_buttonList.append(ui->salve_02_ctr03);
    m_buttonList.append(ui->salve_02_ctr04);
    m_buttonList.append(ui->salve_02_ctr05);
    m_buttonList.append(ui->salve_02_ctr06);

    m_buttonList.append(ui->salve_03_ctr01);
    m_buttonList.append(ui->salve_03_ctr02);
    m_buttonList.append(ui->salve_03_ctr03);
    m_buttonList.append(ui->salve_03_ctr04);
    m_buttonList.append(ui->salve_03_ctr05);
    m_buttonList.append(ui->salve_03_ctr06);

    m_buttonList.append(ui->salve_04_ctr01);
    m_buttonList.append(ui->salve_04_ctr02);
    m_buttonList.append(ui->salve_04_ctr03);
    m_buttonList.append(ui->salve_04_ctr04);
    m_buttonList.append(ui->salve_04_ctr05);
    m_buttonList.append(ui->salve_04_ctr06);

    m_buttonList.append(ui->salve_05_ctr01);
    m_buttonList.append(ui->salve_05_ctr02);
    m_buttonList.append(ui->salve_05_ctr03);
    m_buttonList.append(ui->salve_05_ctr04);
    m_buttonList.append(ui->salve_05_ctr05);
    m_buttonList.append(ui->salve_05_ctr06);

    m_vol_cur_LabelList.append(ui->salve_01_ch01_vol);
    m_vol_cur_LabelList.append(ui->salve_01_ch01_cur);
    m_vol_cur_LabelList.append(ui->salve_01_ch02_vol);
    m_vol_cur_LabelList.append(ui->salve_01_ch02_cur);
    m_vol_cur_LabelList.append(ui->salve_01_ch03_vol);
    m_vol_cur_LabelList.append(ui->salve_01_ch03_cur);

    m_vol_cur_LabelList.append(ui->salve_02_ch01_vol);
    m_vol_cur_LabelList.append(ui->salve_02_ch01_cur);
    m_vol_cur_LabelList.append(ui->salve_02_ch02_vol);
    m_vol_cur_LabelList.append(ui->salve_02_ch02_cur);
    m_vol_cur_LabelList.append(ui->salve_02_ch03_vol);
    m_vol_cur_LabelList.append(ui->salve_02_ch03_cur);

    m_vol_cur_LabelList.append(ui->salve_03_ch01_vol);
    m_vol_cur_LabelList.append(ui->salve_03_ch01_cur);
    m_vol_cur_LabelList.append(ui->salve_03_ch02_vol);
    m_vol_cur_LabelList.append(ui->salve_03_ch02_cur);
    m_vol_cur_LabelList.append(ui->salve_03_ch03_vol);
    m_vol_cur_LabelList.append(ui->salve_03_ch03_cur);

    m_vol_cur_LabelList.append(ui->salve_04_ch01_vol);
    m_vol_cur_LabelList.append(ui->salve_04_ch01_cur);
    m_vol_cur_LabelList.append(ui->salve_04_ch02_vol);
    m_vol_cur_LabelList.append(ui->salve_04_ch02_cur);
    m_vol_cur_LabelList.append(ui->salve_04_ch03_vol);
    m_vol_cur_LabelList.append(ui->salve_04_ch03_cur);

    m_vol_cur_LabelList.append(ui->salve_05_ch01_vol);
    m_vol_cur_LabelList.append(ui->salve_05_ch01_cur);
    m_vol_cur_LabelList.append(ui->salve_05_ch02_vol);
    m_vol_cur_LabelList.append(ui->salve_05_ch02_cur);
    m_vol_cur_LabelList.append(ui->salve_05_ch03_vol);
    m_vol_cur_LabelList.append(ui->salve_05_ch03_cur);

    m_salve_name_LabelList.append(ui->slave01);
    m_salve_name_LabelList.append(ui->slave02);
    m_salve_name_LabelList.append(ui->slave03);
    m_salve_name_LabelList.append(ui->slave04);
    m_salve_name_LabelList.append(ui->slave05);

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


    m_comm_send_timer = new QTimer(this);
    connect(m_comm_send_timer, SIGNAL(timeout()), this, SLOT(on_comm_send_timeout()));
    m_comm_send_timer->start(10000);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    m_timer->start(1000);

    m_master_timer = new QTimer(this);
    connect(m_master_timer, SIGNAL(timeout()), this, SLOT(on_master_timeout()));

    m_slave_timer = new QTimer(this);
    connect(m_slave_timer, SIGNAL(timeout()), this, SLOT(on_slave_timeout()));
    m_slave_timer->start(3000);
//    m_salve_name_LabelList.at(m_id)->setStyleSheet("background-color: green;");

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
//    for(int i = 0; i < parts.size(); i++)
//    {
//        qDebug()<<parts[i];
//    }
    qDebug()<<parts[0];
    qDebug()<<parts[1];

//    m_vol_cur_LabelList.at(m_id*6 + index_ch*2)->setText(parts[0]);
//    m_vol_cur_LabelList.at(m_id*6 + index_ch*2+1)->setText(parts[1]);

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
        m_CH_value[index_ch][i] = (int)measurements.at(i).value;
    }
    qDebug()<<m_CH_value[index_ch][0]<<" "<<m_CH_value[index_ch][1]<<" "<<m_CH_value[index_ch][2]<<" "<<m_CH_value[index_ch][3]<<" "<<m_CH_value[index_ch][4]<<" "<<m_CH_value[index_ch][5];
#endif
    for (const auto &m : measurements) {
        qDebug() << m.name << ":" << m.value << m.unit;
    }
    static int temp_value = 1;
    if(temp_value >= 999)
    {
        temp_value = 1;
    }
    m_vol_cur_LabelList.at(m_id*6 + index_ch*2)->setText(QString::number(m_CH_value[index_ch][0]+temp_value)+"V");
    m_vol_cur_LabelList.at(m_id*6 + index_ch*2+1)->setText(QString::number(m_CH_value[index_ch][1]+temp_value)+"A");
    temp_value += temp_value;
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
    int ret = 0;
    int vol_cur[6];
    int ctl_buff[5];
    memset(ctl_buff, 0, sizeof(ctl_buff));
    if(str.contains("+ADDR="))
    {
        QStringList parts = str.split("=");
        if(parts.size() >= 2)
        {
            m_ID = parts[1];
            m_id = parts[1].toLongLong(&ok, 16);
            m_id = m_id -1;
            qDebug()<<"m_ID:"<<m_ID;
            qDebug()<<"m_id:"<<m_id;
            m_salve_name_LabelList.at(m_id)->setStyleSheet("background-color: green;");
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

        rev_id = parts[1].toLongLong(&ok, 16);
        if(!ok)
        {
            qDebug()<<"change ID to Hex fail.";
        }
        rev_id = rev_id-1;//计算得到真实的ID
        qDebug()<<"rev_id:"<<rev_id;

        // 解析设备发送的数据长度
        rev_len = parts[2].toLongLong(&ok, 16);
        if(!ok)
        {
            qDebug()<<"change Len to Hex fail.";
        }
        qDebug()<<"rev_len:"<<rev_len;

        // 解析设备的数据
        QByteArray rev_data = parts[3].toUtf8();
        const char* charPtr = rev_data.constData();
        if(check_data(charPtr, rev_len) != 0)
        {
            qDebug()<<"rev data check fail.";
//            return -1;
        }
        printf("charPtr=%s\n",charPtr);


        // 计算出电压、电流数据
        for(int i = 0; i < 6; i++)
        {
            vol_cur[i] = (charPtr[i*3+1]-'0')*100 + (charPtr[i*3+2]-'0')*10 + (charPtr[i*3+3]-'0');
        }
        printf("vol:%d %d %d %d %d %d\n", vol_cur[0], vol_cur[1], vol_cur[2], vol_cur[3], vol_cur[4], vol_cur[5]);

        // 计算出开关信息
        for(int i = 0; i < 5; i++)
        {
            ctl_buff[i] = (charPtr[(i+6)*3+1]-'0')*100 + (charPtr[(i+6)*3+2]-'0')*10 + (charPtr[(i+6)*3+3]-'0');
        }
        printf("ctl:%d %d %d %d %d\n", ctl_buff[0], ctl_buff[1], ctl_buff[2], ctl_buff[3], ctl_buff[4]);

        // 更新UI的电压电流信息
        m_vol_cur_LabelList.at(6*(rev_id)+0)->setText(QLocale().toString(vol_cur[0]) + "V");
        m_vol_cur_LabelList.at(6*(rev_id)+2)->setText(QLocale().toString(vol_cur[2]) + "V");
        m_vol_cur_LabelList.at(6*(rev_id)+4)->setText(QLocale().toString(vol_cur[4]) + "V");

        m_vol_cur_LabelList.at(6*(rev_id)+1)->setText(QLocale().toString(vol_cur[1]) + "A");
        m_vol_cur_LabelList.at(6*(rev_id)+3)->setText(QLocale().toString(vol_cur[3]) + "A");
        m_vol_cur_LabelList.at(6*(rev_id)+5)->setText(QLocale().toString(vol_cur[5]) + "A");

        // 比较开关信息，并更新开关信息
        for(int i = 0; i < 5; i++)
        {
            for(int j = 0; j <6; j++)
            {
                if((((m_ctl_value[i]>>j)&1) == ((ctl_buff[i]>>j)&1)) || (m_click_flag[i]>>j&1))
                {
                    continue;
                }
                m_ctl_value[i] &=~(1<<j);
                m_ctl_value[i] |= ctl_buff[i] & (1<<j);
            }
        }
        ret = up_ctl_status();
        if(ret == -1)
        {
            qDebug()<<"up_ctl_status fail.";
        }
        m_comm_send_timer->stop();
//        m_comm_send_timer->start((rev_id-m_id-1)*1000+20);
        if(rev_id > m_id)
        {
            m_comm_send_timer->start((rev_id-m_id-1)*1000+20);
        }
        else
        {
            m_comm_send_timer->start((5-(m_id-rev_id)-1)*1000+20);
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

// 不管是设置输出还是读取输入，最后都需要读取输入。
// 所以return返回的都是读取输入的值(0/1)或者-1
int MainWindow::run_ctl_cmd(int dir, int index, int value)
{
    bool ok;
    int ret = 0;
    QProcess process;
    QProcess process_read;
    process.start();
    if(dir == ENUM_TYPE_OUT)
    {
        if(value)
        {
            process.start("bash", QStringList() << "-c" << m_ctl_cmd.at(index+0*6));
            //process.start("bash", QStringList() << "-c" << QString("echo %1 > /sys/class/gpio/gpio%2/value")
        }
        else
        {
            process.start("bash", QStringList() << "-c" << m_ctl_cmd.at(index+1*6));
//            process.start(m_ctl_cmd.at(index+1*6));
        }
    }
    process.waitForFinished();
    if(process.exitCode() != 0)
    {
        qDebug() << "Command failed:" << process.readAllStandardError();
    }
    cust_delay(5);
    qDebug()<<"cmd:"<<m_ctl_cmd.at(index+2*6);
    process_read.start("bash", QStringList() << "-c" << m_ctl_cmd.at(index+2*6));
    process_read.waitForFinished();
    if (process_read.exitCode() == 0)
    {
        QString result = process_read.readAllStandardOutput();
        qDebug() << "System info:" << result;
        ret = result.toInt(&ok);
        if(ok)
        {
            if(dir == ENUM_TYPE_OUT && ret != value)
            {
                qDebug("m_id: %lld set ctl: %d to %d fail.\n", m_id, index, value);
            }
            return ret;
        }
    }
    else
    {
        qDebug() << "read Command failed:" << process_read.readAllStandardError();
    }

    return -1;
}

// 更新控制开关按钮的状态
int MainWindow::up_ctl_status()
{
    int ret = 0;
    // 按位遍历各个开关按钮
    for(int i = 0; i < 5; i++)
    {
        qDebug()<<"value:"<<i<<"="<<m_ctl_value[i];
        for(int j = 0; j < 6; j++)
        {
            // 如果值为开
            if(m_ctl_value[i]>>j & 1)
            {
                // 检测是否为本机配置
                if(m_id == i)
                {
                    // 如果是本机配置，更新GPIO的配置，并确认更新后的值，根据更新后的值更新UI
                    ret = run_ctl_cmd(ENUM_TYPE_OUT, j, 1);
                    if(ret == 1)
                    {
                        qDebug()<<"1 开";
                        m_buttonList.at(i*6+j)->setText("开");
                    }
                    else
                    {
                        qDebug()<<"1 关";
                        ret = -1;
                        m_buttonList.at(i*6+j)->setText("关");
                    }
                }
                else
                {
                    // 非本机配置，直接更新UI
                    m_buttonList.at(i*6+j)->setText("开");
                }
            }
            else
            {
                if(m_id == i)
                {
                    ret = run_ctl_cmd(ENUM_TYPE_OUT, j, 0);
                    if(ret == 0)
                    {
                        qDebug()<<"0 关";
                        m_buttonList.at(i*6+j)->setText("关");
                    }
                    else
                    {
                        qDebug()<<"0 开";
                        ret = -1;
                        m_buttonList.at(i*6+j)->setText("开");
                    }
                }
                else
                {
                    m_buttonList.at(i*6+j)->setText("关");
                }
            }
        }
    }
    return ret;
}

// | Vrms:   0.00000V | Irms:   0.00000A | P:   0.0000W | PF: 0.00000 | F:  0.0000Hz | W:   0.0051KW*H |
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
    parse_comm_data(QString(read_buff));
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

void MainWindow::on_comm_send_timeout()
{
    QString str1="AT+TX=FFFFFFFFFFFF,0023,";
    QString str2=str1+"A";
    QString str3=str2+QString("%1").arg(m_CH_value[0][0], 3, 10, QChar('0'));
    QString str4=str3+QString("%1").arg(m_CH_value[0][1], 3, 10, QChar('0'));
    QString str5=str4+QString("%1").arg(m_CH_value[1][0], 3, 10, QChar('0'));
    QString str6=str5+QString("%1").arg(m_CH_value[1][1], 3, 10, QChar('0'));
    QString str7=str6+QString("%1").arg(m_CH_value[2][0], 3, 10, QChar('0'));
    QString str8=str7+QString("%1").arg(m_CH_value[2][1], 3, 10, QChar('0'));
    QString str9=str8+QString("%1").arg(m_ctl_value[0], 3, 10, QChar('0'));
    QString str10=str9+QString("%1").arg(m_ctl_value[1], 3, 10, QChar('0'));
    QString str11=str10+QString("%1").arg(m_ctl_value[2], 3, 10, QChar('0'));
    QString str12=str11+QString("%1").arg(m_ctl_value[3], 3, 10, QChar('0'));
    QString str13=str12+QString("%1").arg(m_ctl_value[4], 3, 10, QChar('0'));
    QString str14=str13+"5";
    QString str15=str14+"\r\n";
    QByteArray byteArray = str15.toUtf8();
    m_serial_power_communication->write(byteArray);
    m_comm_send_timer->stop();
    m_comm_send_timer->start((5-1)*1000+20);
    memset(&m_click_flag, 0, sizeof(m_click_flag));
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
    qDebug()<<"on_salve_01_ctr01_clicked";
    m_click_flag[0] = m_click_flag[0] | 1<<0;
    m_ctl_value[0] ^=1<<0;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_01_ctr01_clicked fail.";
    }
}

void MainWindow::on_salve_01_ctr02_clicked()
{
    m_click_flag[0] = m_click_flag[0] | 1<<1;
    m_ctl_value[0] ^=1<<1;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_01_ctr02_clicked fail.";
    }

}

void MainWindow::on_salve_01_ctr03_clicked()
{
    m_click_flag[0] = m_click_flag[0] | 1<<2;
    m_ctl_value[0] ^=1<<2;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_01_ctr03_clicked fail.";
    }

}

void MainWindow::on_salve_01_ctr04_clicked()
{
    m_click_flag[0] = m_click_flag[0] | 1<<3;
    m_ctl_value[0] ^=1<<3;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_01_ctr04_clicked fail.";
    }

}

void MainWindow::on_salve_01_ctr05_clicked()
{
    m_click_flag[0] = m_click_flag[0] | 1<<4;
    m_ctl_value[0] ^=1<<4;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_01_ctr05_clicked fail.";
    }

}

void MainWindow::on_salve_01_ctr06_clicked()
{
    m_click_flag[0] = m_click_flag[0] | 1<<5;
    m_ctl_value[0] ^=1<<5;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_01_ctr06_clicked fail.";
    }

}

void MainWindow::on_salve_02_ctr01_clicked()
{
    m_click_flag[1] = m_click_flag[1] | 1<<0;
    m_ctl_value[1] ^=1<<0;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_02_ctr01_clicked fail.";
    }

}

void MainWindow::on_salve_02_ctr02_clicked()
{
    m_click_flag[1] = m_click_flag[1] | 1<<1;
    m_ctl_value[1] ^=1<<1;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_02_ctr02_clicked fail.";
    }

}

void MainWindow::on_salve_02_ctr03_clicked()
{
    m_click_flag[1] = m_click_flag[1] | 1<<2;
    m_ctl_value[1] ^=1<<2;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_02_ctr03_clicked fail.";
    }

}

void MainWindow::on_salve_02_ctr04_clicked()
{
    m_click_flag[1] = m_click_flag[1] | 1<<3;
    m_ctl_value[1] ^=1<<3;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_02_ctr04_clicked fail.";
    }

}

void MainWindow::on_salve_02_ctr05_clicked()
{
    m_click_flag[1] = m_click_flag[1] | 1<<4;
    m_ctl_value[1] ^=1<<4;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_02_ctr05_clicked fail.";
    }

}

void MainWindow::on_salve_02_ctr06_clicked()
{
    m_click_flag[1] = m_click_flag[1] | 1<<5;
    m_ctl_value[1] ^=1<<5;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_02_ctr06_clicked fail.";
    }

}

void MainWindow::on_salve_03_ctr01_clicked()
{
    m_click_flag[2] = m_click_flag[2] | 1<<0;
    m_ctl_value[2] ^=1<<0;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_03_ctr01_clicked fail.";
    }

}

void MainWindow::on_salve_03_ctr02_clicked()
{
    m_click_flag[2] = m_click_flag[2] | 1<<1;
    m_ctl_value[2] ^=1<<1;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_03_ctr02_clicked fail.";
    }

}

void MainWindow::on_salve_03_ctr03_clicked()
{
    m_click_flag[2] = m_click_flag[2] | 1<<2;
    m_ctl_value[2] ^=1<<2;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_03_ctr03_clicked fail.";
    }

}

void MainWindow::on_salve_03_ctr04_clicked()
{
    m_click_flag[2] = m_click_flag[2] | 1<<3;
    m_ctl_value[2] ^=1<<3;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_03_ctr04_clicked fail.";
    }

}

void MainWindow::on_salve_03_ctr05_clicked()
{
    m_click_flag[2] = m_click_flag[2] | 1<<4;
    m_ctl_value[2] ^=1<<4;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_03_ctr05_clicked fail.";
    }

}

void MainWindow::on_salve_03_ctr06_clicked()
{
    m_click_flag[2] = m_click_flag[2] | 1<<5;
    m_ctl_value[2] ^=1<<5;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_03_ctr06_clicked fail.";
    }

}

void MainWindow::on_salve_04_ctr01_clicked()
{
    m_click_flag[3] = m_click_flag[3] | 1<<0;
    m_ctl_value[3] ^=1<<0;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_04_ctr01_clicked fail.";
    }

}

void MainWindow::on_salve_04_ctr02_clicked()
{
    m_click_flag[3] = m_click_flag[3] | 1<<1;
    m_ctl_value[3] ^=1<<1;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_04_ctr02_clicked fail.";
    }

}

void MainWindow::on_salve_04_ctr03_clicked()
{
    m_click_flag[3] = m_click_flag[3] | 1<<2;
    m_ctl_value[3] ^=1<<2;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_04_ctr03_clicked fail.";
    }

}

void MainWindow::on_salve_04_ctr04_clicked()
{
    m_click_flag[3] = m_click_flag[3] | 1<<3;
    m_ctl_value[3] ^=1<<3;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_04_ctr04_clicked fail.";
    }

}

void MainWindow::on_salve_04_ctr05_clicked()
{
    m_click_flag[3] = m_click_flag[3] | 1<<4;
    m_ctl_value[3] ^=1<<4;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_04_ctr05_clicked fail.";
    }

}

void MainWindow::on_salve_04_ctr06_clicked()
{
    m_click_flag[3] = m_click_flag[3] | 1<<5;
    m_ctl_value[3] ^=1<<5;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_04_ctr06_clicked fail.";
    }

}

void MainWindow::on_salve_05_ctr01_clicked()
{
    m_click_flag[4] = m_click_flag[4] | 1<<0;
    m_ctl_value[4] ^=1<<0;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_05_ctr01_clicked fail.";
    }

}

void MainWindow::on_salve_05_ctr02_clicked()
{
    m_click_flag[4] = m_click_flag[4] | 1<<1;
    m_ctl_value[4] ^=1<<1;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_05_ctr02_clicked fail.";
    }

}

void MainWindow::on_salve_05_ctr03_clicked()
{
    m_click_flag[4] = m_click_flag[4] | 1<<2;
    m_ctl_value[4] ^=1<<2;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_05_ctr03_clicked fail.";
    }

}

void MainWindow::on_salve_05_ctr04_clicked()
{
    m_click_flag[4] = m_click_flag[4] | 1<<3;
    m_ctl_value[4] ^=1<<3;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_05_ctr04_clicked fail.";
    }

}

void MainWindow::on_salve_05_ctr05_clicked()
{
    m_click_flag[4] = m_click_flag[4] | 1<<4;
    m_ctl_value[4] ^=1<<4;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_05_ctr05_clicked fail.";
    }

}

void MainWindow::on_salve_05_ctr06_clicked()
{
    m_click_flag[4] = m_click_flag[4] | 1<<5;
    m_ctl_value[4] ^=1<<5;
    if(up_ctl_status() == -1)
    {
        qDebug()<<"on_salve_05_ctr06_clicked fail.";
    }
}
