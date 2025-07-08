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
#if USE_DEBUG
    m_serial = new QSerialPort();
    m_serial->setPortName(SERIAL_1_NAME);
    m_serial->open(QIODevice::ReadWrite);
    m_serial->setBaudRate(QSerialPort::Baud115200);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(m_serial, SIGNAL(readyRead()), this, SLOT(read_serial_data()));
#else
    m_comm_rev_flag = 0; //初始化flag标志
    m_serial_vol_curr_CH01 = new QSerialPort();
    m_serial_vol_curr_CH01->setPortName(SERIAL_VOL_CURR_CH01_NAME);
    m_serial_vol_curr_CH01->open(QIODevice::ReadWrite);
    m_serial_vol_curr_CH01->setBaudRate(QSerialPort::Baud9600);
//    m_serial_vol_curr_CH01->setBaudRate(QSerialPort::Baud9600);
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
#endif

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
    m_timer->start(2000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::parseMeasurementData(int index_salve, QString inputs)
{
    QString test1 = inputs.remove(' ');
    QString input = test1.split("\r\n").first();
    // 使用"|"分割字符串并去除空项
    QStringList parts = input.split("|", QString::SkipEmptyParts);
    for(int i = 0; i < parts.size(); i++)
    {
        qDebug()<<parts[i];
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
        m_CH_value[index_salve][i] = measurements.at(i).value;
    }
    qDebug()<<m_CH_value[index_salve][0]<<" "<<m_CH_value[index_salve][1]<<" "<<m_CH_value[index_salve][2]<<" "<<m_CH_value[index_salve][3]<<" "<<m_CH_value[index_salve][4]<<" "<<m_CH_value[index_salve][5];
#endif
    for (const auto &m : measurements) {
        qDebug() << m.name << ":" << m.value << m.unit;
    }

}

#if USE_DEBUG
void MainWindow::read_serial_data()
{
    QByteArray read_buff;
    read_buff = m_serial->readAll();
//    ui->textBrowser->setText(read_buff.toStdString());
    ui->textBrowser->setText(QString(read_buff));
    qDebug("read:%s\n", qPrintable(QString(read_buff)));
}


void MainWindow::on_pushButton_clicked()
{
    QByteArray send_data = ui->textEdit->toPlainText().toUtf8();
    m_serial->write(send_data);
}
#else

void MainWindow::read_serial_vol_curr_CH01()
{
    QByteArray read_buff;
    cust_delay(1000);
    read_buff = m_serial_vol_curr_CH01->readAll();
    qDebug("CH01 read:%s\n", qPrintable(QString(read_buff)));
    parseMeasurementData(0, QString(read_buff));

}

void MainWindow::read_serial_vol_curr_CH02()
{
    QByteArray read_buff;
    cust_delay(1000);
    read_buff = m_serial_vol_curr_CH02->readAll();
    qDebug("CH02 read:%s\n", qPrintable(QString(read_buff)));
    parseMeasurementData(1, QString(read_buff));
}

void MainWindow::read_serial_vol_curr_CH03()
{
    QByteArray read_buff;
    cust_delay(1000);
    read_buff = m_serial_vol_curr_CH03->readAll();
    qDebug("CH03 read:%s\n", qPrintable(QString(read_buff)));
    parseMeasurementData(2, QString(read_buff));
}

void MainWindow::read_serial_power_communication()
{
    QByteArray read_buff;
    read_buff = m_serial_power_communication->readAll();
    qDebug("COMM read:%s\n", qPrintable(QString(read_buff)));
}
#endif

void MainWindow::on_timeout()
{
    qDebug()<<"this is timeout";
#if USE_DEBUG
#else
    m_timer->stop();
    if(m_comm_rev_flag == 0)
    {

    }
//    m_serial_vol_curr_CH01->write(" | Vrms: 221.0V | Irms: 0.1A | P: 0.0000W | PF: 0.00000 | F: 0.0000Hz | W: 0.0035KW*H |");
//    m_serial_vol_curr_CH02->write(" | Vrms: 222.12V | Irms: 0.2A | P: 0.0000W | PF: 0.00000 | F: 0.0000Hz | W: 0.0035KW*H |");
//    m_serial_vol_curr_CH03->write(" | Vrms: 223.1V | Irms: 0.3A | P: 0.0000W | PF: 0.00000 | F: 0.0000Hz | W: 0.0035KW*H |");
    m_serial_vol_curr_CH01->write(">>GetVal");
    m_serial_vol_curr_CH02->write(">>GetVal");
    m_serial_vol_curr_CH03->write(">>GetVal");
    m_serial_power_communication->write("I am is COMM");
    m_timer->start(2000);
#endif
}
