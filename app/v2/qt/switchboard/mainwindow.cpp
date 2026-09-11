#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QByteArray>

#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QFile>

#define SUB_STATUS_A    (1)
#define SUB_STATUS_B    (2)
#define SUB_STATUS_C    (3)

#define SUB_STATUS_GREEN    (1)
#define SUB_STATUS_YELLOW    (2)

#define PASSWORD_PATH ("password.bat")

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    get_password();
    m_config_ui = new config_window();
    QObject::connect(this, &MainWindow::sen_set_id_return, m_config_ui, &config_window::get_set_id_return, Qt::QueuedConnection);
    QObject::connect(m_config_ui, &config_window::sen_set_id_value, this, &MainWindow::get_set_id_value, Qt::QueuedConnection);


    QObject::connect(this, &MainWindow::sen_set_password_return, m_config_ui, &config_window::get_set_password_return, Qt::QueuedConnection);
    QObject::connect(m_config_ui, &config_window::sen_set_password_value, this, &MainWindow::get_set_password_value, Qt::QueuedConnection);

    m_connect_flag = 0;
    m_total_len = 0;
    memset(sub_status, 1, sizeof (sub_status));
    memset(total_buff, 1, sizeof (total_buff));
    this->showFullScreen();
    ui->setupUi(this);
    ui->pushButton_17->hide();
    ui->pushButton_18->hide();
    ui->pushButton_19->hide();
    m_tcpsocket = new QTcpSocket(this);
    connect(m_tcpsocket, SIGNAL(connected()), this, SLOT(connected_slot()));
    connect(m_tcpsocket, SIGNAL(disconnected()), this, SLOT(disconnect_slot()));

    m_buttonList.append(ui->pushButton_1_A);
    m_buttonList.append(ui->pushButton_1_B);
    m_buttonList.append(ui->pushButton_1_C);

    m_buttonList.append(ui->pushButton_2_A);
    m_buttonList.append(ui->pushButton_2_B);
    m_buttonList.append(ui->pushButton_2_C);

    m_buttonList.append(ui->pushButton_3_A);
    m_buttonList.append(ui->pushButton_3_B);
    m_buttonList.append(ui->pushButton_3_C);

    m_buttonList.append(ui->pushButton_4_A);
    m_buttonList.append(ui->pushButton_4_B);
    m_buttonList.append(ui->pushButton_4_C);

    m_buttonList.append(ui->pushButton_5_A);
    m_buttonList.append(ui->pushButton_5_B);
    m_buttonList.append(ui->pushButton_5_C);

    m_vol_cur_LabelList.append(ui->label_0_0V);
    m_vol_cur_LabelList.append(ui->label_0_0A);
    m_vol_cur_LabelList.append(ui->label_0_1V);
    m_vol_cur_LabelList.append(ui->label_0_1A);
    m_vol_cur_LabelList.append(ui->label_0_2V);
    m_vol_cur_LabelList.append(ui->label_0_2A);

    m_vol_cur_LabelList.append(ui->label_1_0V);
    m_vol_cur_LabelList.append(ui->label_1_0A);
    m_vol_cur_LabelList.append(ui->label_1_1V);
    m_vol_cur_LabelList.append(ui->label_1_1A);
    m_vol_cur_LabelList.append(ui->label_1_2V);
    m_vol_cur_LabelList.append(ui->label_1_2A);

    m_vol_cur_LabelList.append(ui->label_2_0V);
    m_vol_cur_LabelList.append(ui->label_2_0A);
    m_vol_cur_LabelList.append(ui->label_2_1V);
    m_vol_cur_LabelList.append(ui->label_2_1A);
    m_vol_cur_LabelList.append(ui->label_2_2V);
    m_vol_cur_LabelList.append(ui->label_2_2A);

    m_vol_cur_LabelList.append(ui->label_3_0V);
    m_vol_cur_LabelList.append(ui->label_3_0A);
    m_vol_cur_LabelList.append(ui->label_3_1V);
    m_vol_cur_LabelList.append(ui->label_3_1A);
    m_vol_cur_LabelList.append(ui->label_3_2V);
    m_vol_cur_LabelList.append(ui->label_3_2A);

    m_vol_cur_LabelList.append(ui->label_4_0V);
    m_vol_cur_LabelList.append(ui->label_4_0A);
    m_vol_cur_LabelList.append(ui->label_4_1V);
    m_vol_cur_LabelList.append(ui->label_4_1A);
    m_vol_cur_LabelList.append(ui->label_4_2V);
    m_vol_cur_LabelList.append(ui->label_4_2A);

    m_vol_cur_LabelList.append(ui->label_5_0V);
    m_vol_cur_LabelList.append(ui->label_5_0A);
    m_vol_cur_LabelList.append(ui->label_5_1V);
    m_vol_cur_LabelList.append(ui->label_5_1A);
    m_vol_cur_LabelList.append(ui->label_5_2V);
    m_vol_cur_LabelList.append(ui->label_5_2A);

//    for(int i = 0; i <m_buttonList.size(); i++)
//    {
//        m_buttonList.at(i)->setDisabled(true);
//    }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_3_clicked()
{
    if(m_connect_flag == 0)
    {
        if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty())
        {
            m_tcpsocket->connectToHost("47.109.24.25", 33306);
        }
        else
        {
            m_tcpsocket->connectToHost(ui->lineEdit->text(), ui->lineEdit_2->text().toUInt());
        }
    }
    else
    {
        qDebug()<<"disconnect";
        m_tcpsocket->close();
    }
}

void MainWindow::connected_slot()
{
    if(m_connect_flag == 0)
    {
        m_connect_flag = 1;
        connect(m_tcpsocket, SIGNAL(readyRead()), this, SLOT(readyRead_slot()));
        ui->pushButton_3->setText("disconnect");
        ui->lineEdit->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
        for(int i = 0; i <m_buttonList.size(); i++)
        {
            m_buttonList.at(i)->setEnabled(true);
        }
    }
    else
    {

    }
}


void MainWindow::disconnect_slot()
{
    if(m_connect_flag == 1)
    {
        m_connect_flag = 0;
        ui->pushButton_3->setText("connect");
        ui->lineEdit->setEnabled(true);
        ui->lineEdit_2->setEnabled(true);
    }
}

#define FRAME_MIN_SIZE (4)
#define BUFFER_SIZE 128
void MainWindow::readyRead_slot()
{
    unsigned char send_buffer[BUFFER_SIZE];
    int copy_data_len = 0;
    unsigned char check_sum = 0;
    int i = 0;
    unsigned char *pstr = NULL;
    int parse_pos = 0;
    int invalid_pos = 0;
    unsigned char frame_len = 0;
    QByteArray data = m_tcpsocket->readAll();
    const char* cdata = data.constData();
    copy_data_len = data.size();

    if(copy_data_len > (int)(sizeof(total_buff)-m_total_len))
    {
        copy_data_len = sizeof (total_buff) - m_total_len;
    }
    memcpy(total_buff + m_total_len, cdata, copy_data_len);
    m_total_len = m_total_len + copy_data_len;
    i = 0;
    parse_pos = 0;
    invalid_pos = 0;
    pstr = NULL;
    while(i < m_total_len)
    {
        if(total_buff[i] == 0xaa)
        {
            parse_pos = i;
        }
        else
        {
            invalid_pos = i;
            i++;
            continue;
        }

        if(i+1 >= m_total_len)
        {
            break;
        }

        frame_len = total_buff[i+1];
        if(frame_len < FRAME_MIN_SIZE)
        {
            // 帧长度不正确，跳过
            i++;
            continue;

        }
        if(frame_len+i > m_total_len)
        {
            break;
        }

        pstr = &total_buff[i];
        check_sum = 0;
        memset(send_buffer, 0, sizeof(send_buffer));
        for(int j = 0; j < frame_len; j++)
        {
            send_buffer[j] = pstr[j];
            if((2 <= j) && (j < (frame_len-1)))
            {
                check_sum += send_buffer[j];
            }
        }

        if(check_sum == send_buffer[frame_len-1])
        {
            // for(int j = 0; j < frame_len; j++)
            // {
            //     printf("%02x ", send_buffer[j]);
            // }
            // printf("\n");
            // boardcast(sock, send_buffer, frame_len);
            update_ui(send_buffer, frame_len);
        }

        i = i + frame_len;
    }

    if((invalid_pos > parse_pos) || (i == m_total_len))
    {
        memset(total_buff, 0, sizeof(total_buff));
        m_total_len = 0;
    }

    if((invalid_pos < parse_pos) && (i < m_total_len))
    {
        for(int j = 0; j < (m_total_len-i); j++)
        {
            total_buff[j] = total_buff[j+i];
        }
        m_total_len = m_total_len - i;
    }

}

int MainWindow::set_sub_status()
{
    for(int i = 0; i < 5; i++)
    {
        if(sub_status[0][i] == SUB_STATUS_A)
        {
            m_buttonList.at(i*3+0)->setText("A");
            if(sub_status[1][i] == SUB_STATUS_GREEN)
            {
                m_buttonList.at(i*3+0)->setStyleSheet("background-color: green;");

            }
            else if(sub_status[1][i] == SUB_STATUS_YELLOW)
            {
                m_buttonList.at(i*3+0)->setStyleSheet("background-color: yellow;");

            }
            m_buttonList.at(i*3+1)->setText("B");
            m_buttonList.at(i*3+2)->setText("C");
            m_buttonList.at(i*3+0)->setEnabled(true);
            m_buttonList.at(i*3+1)->setEnabled(true);
            m_buttonList.at(i*3+2)->setEnabled(true);
        }
        else if(sub_status[0][i] == SUB_STATUS_B)
        {
            m_buttonList.at(i*3+0)->setText("B");
            if(sub_status[1][i] == SUB_STATUS_GREEN)
            {
                m_buttonList.at(i*3+0)->setStyleSheet("background-color: green;");

            }
            else if(sub_status[1][i] == SUB_STATUS_YELLOW)
            {
                m_buttonList.at(i*3+0)->setStyleSheet("background-color: yellow;");

            }
            m_buttonList.at(i*3+1)->setText("C");
            m_buttonList.at(i*3+2)->setText("A");
            m_buttonList.at(i*3+0)->setEnabled(true);
            m_buttonList.at(i*3+1)->setEnabled(true);
            m_buttonList.at(i*3+2)->setEnabled(true);

        }
        else if(sub_status[0][i] == SUB_STATUS_C)
        {
            m_buttonList.at(i*3+0)->setText("C");
            if(sub_status[1][i] == SUB_STATUS_GREEN)
            {
                m_buttonList.at(i*3+0)->setStyleSheet("background-color: green;");

            }
            else if(sub_status[1][i] == SUB_STATUS_YELLOW)
            {
                m_buttonList.at(i*3+0)->setStyleSheet("background-color: yellow;");

            }
            m_buttonList.at(i*3+1)->setText("A");
            m_buttonList.at(i*3+2)->setText("B");
            m_buttonList.at(i*3+0)->setEnabled(true);
            m_buttonList.at(i*3+1)->setEnabled(true);
            m_buttonList.at(i*3+2)->setEnabled(true);
        }
        else
        {
            m_buttonList.at(i*3+0)->setDisabled(true);
            m_buttonList.at(i*3+0)->setStyleSheet("");
            m_buttonList.at(i*3+1)->setDisabled(true);
            m_buttonList.at(i*3+2)->setDisabled(true);
        }
    }
    return 0;
}
#define INFO_FRAME_LEN  (81)
int MainWindow::update_ui(unsigned char *data, int len)
{
    unsigned char *ptr = NULL;
    int value[6];
    if(len != INFO_FRAME_LEN)
    {
        return -1;
    }

    ptr = &data[2];
    for(int i = 0; i < 6; i++)
    {
        value[0] = ptr[i*13+0]*256+ptr[i*13+1];
        value[1] = ptr[i*13+2]*256+ptr[i*13+3];
        value[2] = ptr[i*13+4]*256+ptr[i*13+5];
        value[3] = ptr[i*13+6]*256+ptr[i*13+7];
        value[4] = ptr[i*13+8]*256+ptr[i*13+9];
        value[5] = ptr[i*13+10]*256+ptr[i*13+11];
        if(i > 0)
        {
            sub_status[0][i-1] = ptr[i*13+12];
            sub_status[1][i-1] = SUB_STATUS_GREEN;
        }
        m_vol_cur_LabelList.at(i*6+0)->setText(QString::number(value[0])+"V");
        m_vol_cur_LabelList.at(i*6+1)->setText(QString::number(value[1])+"A");
        m_vol_cur_LabelList.at(i*6+2)->setText(QString::number(value[2])+"V");
        m_vol_cur_LabelList.at(i*6+3)->setText(QString::number(value[3])+"A");
        m_vol_cur_LabelList.at(i*6+4)->setText(QString::number(value[4])+"V");
        m_vol_cur_LabelList.at(i*6+5)->setText(QString::number(value[5])+"A");
    }
    set_sub_status();

    return 0;
}


// | Vrms:   0.00000V | Irms:   0.00000A | P:   0.0000W | PF: 0.00000 | F:  0.0000Hz | W:   0.0051KW*H |
void MainWindow::read_serial_vol_curr_CH01()
{
//    QByteArray read_buff;
//    cust_delay(1000);
//    read_buff = m_serial_vol_curr_CH01->readAll();
//    qDebug("CH01 read:%s\n", qPrintable(QString(read_buff)));
//    parse_vol_cur_data(0, QString(read_buff));

}

void MainWindow::read_serial_vol_curr_CH02()
{
//    QByteArray read_buff;
//    cust_delay(1000);
//    read_buff = m_serial_vol_curr_CH02->readAll();
//    qDebug("CH02 read:%s\n", qPrintable(QString(read_buff)));
//    parse_vol_cur_data(1, QString(read_buff));
}

void MainWindow::read_serial_vol_curr_CH03()
{
//    QByteArray read_buff;
//    cust_delay(1000);
//    read_buff = m_serial_vol_curr_CH03->readAll();
//    qDebug("CH03 read:%s\n", qPrintable(QString(read_buff)));
//    parse_vol_cur_data(2, QString(read_buff));
}

void MainWindow::read_serial_power_communication()
{
//    QByteArray read_buff;
//    cust_delay(120);
//    m_comm_rev_flag = 1;        // 电力载波通信有收到数据
//    read_buff = m_serial_power_communication->readAll();
//    qDebug("COMM read:%s\n", qPrintable(QString(read_buff)));
//    parse_comm_data(QString(read_buff));
}

int MainWindow::init_serialport()
{
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
}

int MainWindow::config_tir_ctl(int status)
{
    if(status == m_sub_status[m_id])
    {
        return 0;
    }
}

int MainWindow::get_password()
{
    QFile file(PASSWORD_PATH);

    // 1. 尝试以读写模式打开文件。若文件不存在，此操作会自动创建它。
    if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
        qDebug() << "打开或创建文件失败:" << file.errorString();
        return -1;
    }

    // 2. 读取文件现有内容
    QTextStream in(&file);
    QString content = in.readAll();

    // 3. 判断文件是否为空（新创建的文件或内容为空）
    if (content.isEmpty()) {
        // 4. 文件为空，写入默认内容
        QString defaultContent = "123456";

        // 注意：此时文件指针在文件末尾，需要 seek 到开头再写入，否则会追加在末尾
        file.seek(0);
        QTextStream out(&file);
        out << defaultContent;
        // 写入后截断文件，确保清除可能存在的旧内容（如果有）
        file.resize(out.pos());

        qDebug() << "文件不存在或为空，已写入默认内容。";
        m_password = defaultContent;
    } else {
        // 5. 文件存在且有内容，进行处理
        qDebug() << "成功读取文件内容:" << content;
        // 在这里添加对 content 的后续处理逻辑
        m_password = content;
    }

    // 6. 关闭文件
    file.close();
    return 0;
}

int MainWindow::change_password(QString password)
{
    QFile file(PASSWORD_PATH);

    // 以 WriteOnly 模式打开（自动清空原有内容，文件不存在则创建）
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "无法打开文件：" << file.errorString();
        return false;
    }

    // 写入新内容
    QTextStream out(&file);
    out << password;

    // 确保数据完全写入磁盘（close 时会自动 flush，但显式调用更安全）
    file.flush();
    file.close();

    return 0;
}


void MainWindow::on_pushButton_1_A_clicked()
{

}

void MainWindow::on_pushButton_1_B_clicked()
{
    unsigned char checksum = 0;

    bool ok;
    QString password = QInputDialog::getText(this, "电相切换操作",
                                             "请输入密码:",
                                             QLineEdit::Password,
                                             "",
                                             &ok);
    if (ok && !password.isEmpty()) {
        if (password == "123456")
        {
            qDebug()<<"pass";
        }
        else
        {
            qDebug()<<"error";
        }
        // 用户点击了确定且输入了密码
        // 在这里校验密码，比如 if (password == "123456") {... }
    } else if (ok && password.isEmpty()) {
        QMessageBox::warning(this, "提示", "密码不能为空！");
    } else {
        qDebug()<<"cancel";
        // 用户点击了取消
    }

    m_mutex.lock();
    if(sub_status[0][0] == SUB_STATUS_A)
    {
        //设置为B
        sub_status[0][0] = SUB_STATUS_B;

    }
    else if(sub_status[0][0] == SUB_STATUS_B)
    {
        // 设置为C
        sub_status[0][0] = SUB_STATUS_C;
    }
    else if(sub_status[0][0] == SUB_STATUS_C)
    {
        // 设置为A
        sub_status[0][0] = SUB_STATUS_A;
    }
    sub_status[1][0] = SUB_STATUS_YELLOW;
    m_send_buff[0] = 0xaa;
    m_send_buff[1] = 0x08;
    for(int i = 0; i < 5; i++)
    {
        m_send_buff[i+2] = sub_status[0][i];
        checksum += m_send_buff[i+2];
    }
    m_send_buff[7] = checksum;

    QByteArray byteArray(reinterpret_cast<const char*>(m_send_buff), sizeof (m_send_buff));
    m_tcpsocket->write(byteArray);
    m_tcpsocket->waitForBytesWritten();
    set_sub_status();
    m_mutex.unlock();

}

void MainWindow::on_pushButton_1_C_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][0] == SUB_STATUS_A)
    {
        // 设置为C
        sub_status[0][0] = SUB_STATUS_C;
    }
    else if(sub_status[0][0] == SUB_STATUS_B)
    {
        // 设置为A
        sub_status[0][0] = SUB_STATUS_A;
    }
    else if(sub_status[0][0] == SUB_STATUS_C)
    {
        // 设置为B
        sub_status[0][0] = SUB_STATUS_B;
    }
    sub_status[1][0] = SUB_STATUS_YELLOW;
    m_send_buff[0] = 0xaa;
    m_send_buff[1] = 0x08;
    for(int i = 0; i < 5; i++)
    {
        m_send_buff[i+2] = sub_status[0][i];
        checksum += m_send_buff[i+2];
    }
    m_send_buff[7] = checksum;

    QByteArray byteArray(reinterpret_cast<const char*>(m_send_buff), sizeof (m_send_buff));
    m_tcpsocket->write(byteArray);
    m_tcpsocket->waitForBytesWritten();
    set_sub_status();
    m_mutex.unlock();

}

void MainWindow::on_pushButton_2_A_clicked()
{

}

void MainWindow::on_pushButton_2_B_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][1] == SUB_STATUS_A)
    {
        //设置为B
        sub_status[0][1] = SUB_STATUS_B;

    }
    else if(sub_status[0][1] == SUB_STATUS_B)
    {
        // 设置为C
        sub_status[0][1] = SUB_STATUS_C;
    }
    else if(sub_status[0][1] == SUB_STATUS_C)
    {
        // 设置为A
        sub_status[0][1] = SUB_STATUS_A;
    }
    sub_status[1][1] = SUB_STATUS_YELLOW;
    m_send_buff[0] = 0xaa;
    m_send_buff[1] = 0x08;
    for(int i = 0; i < 5; i++)
    {
        m_send_buff[i+2] = sub_status[0][i];
        checksum += m_send_buff[i+2];
    }
    m_send_buff[7] = checksum;

    QByteArray byteArray(reinterpret_cast<const char*>(m_send_buff), sizeof (m_send_buff));
    m_tcpsocket->write(byteArray);
    m_tcpsocket->waitForBytesWritten();
    set_sub_status();
    m_mutex.unlock();

}

void MainWindow::on_pushButton_2_C_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][1] == SUB_STATUS_A)
    {
        // 设置为C
        sub_status[0][1] = SUB_STATUS_C;
    }
    else if(sub_status[0][1] == SUB_STATUS_B)
    {
        // 设置为A
        sub_status[0][1] = SUB_STATUS_A;
    }
    else if(sub_status[0][1] == SUB_STATUS_C)
    {
        // 设置为B
        sub_status[0][1] = SUB_STATUS_B;
    }
    sub_status[1][1] = SUB_STATUS_YELLOW;
    m_send_buff[0] = 0xaa;
    m_send_buff[1] = 0x08;
    for(int i = 0; i < 5; i++)
    {
        m_send_buff[i+2] = sub_status[0][i];
        checksum += m_send_buff[i+2];
    }
    m_send_buff[7] = checksum;

    QByteArray byteArray(reinterpret_cast<const char*>(m_send_buff), sizeof (m_send_buff));
    m_tcpsocket->write(byteArray);
    m_tcpsocket->waitForBytesWritten();
    set_sub_status();
    m_mutex.unlock();

}

void MainWindow::on_pushButton_3_A_clicked()
{

}

void MainWindow::on_pushButton_3_B_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][2] == SUB_STATUS_A)
    {
        //设置为B
        sub_status[0][2] = SUB_STATUS_B;

    }
    else if(sub_status[0][2] == SUB_STATUS_B)
    {
        // 设置为C
        sub_status[0][2] = SUB_STATUS_C;
    }
    else if(sub_status[0][2] == SUB_STATUS_C)
    {
        // 设置为A
        sub_status[0][2] = SUB_STATUS_A;
    }
    sub_status[1][2] = SUB_STATUS_YELLOW;
    m_send_buff[0] = 0xaa;
    m_send_buff[1] = 0x08;
    for(int i = 0; i < 5; i++)
    {
        m_send_buff[i+2] = sub_status[0][i];
        checksum += m_send_buff[i+2];
    }
    m_send_buff[7] = checksum;

    QByteArray byteArray(reinterpret_cast<const char*>(m_send_buff), sizeof (m_send_buff));
    m_tcpsocket->write(byteArray);
    m_tcpsocket->waitForBytesWritten();
    set_sub_status();
    m_mutex.unlock();

}

void MainWindow::on_pushButton_3_C_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][2] == SUB_STATUS_A)
    {
        // 设置为C
        sub_status[0][2] = SUB_STATUS_C;
    }
    else if(sub_status[0][2] == SUB_STATUS_B)
    {
        // 设置为A
        sub_status[0][2] = SUB_STATUS_A;
    }
    else if(sub_status[0][2] == SUB_STATUS_C)
    {
        // 设置为B
        sub_status[0][2] = SUB_STATUS_B;
    }
    sub_status[1][2] = SUB_STATUS_YELLOW;
    m_send_buff[0] = 0xaa;
    m_send_buff[1] = 0x08;
    for(int i = 0; i < 5; i++)
    {
        m_send_buff[i+2] = sub_status[0][i];
        checksum += m_send_buff[i+2];
    }
    m_send_buff[7] = checksum;

    QByteArray byteArray(reinterpret_cast<const char*>(m_send_buff), sizeof (m_send_buff));
    m_tcpsocket->write(byteArray);
    m_tcpsocket->waitForBytesWritten();
    set_sub_status();
    m_mutex.unlock();

}

void MainWindow::on_pushButton_4_A_clicked()
{

}

void MainWindow::on_pushButton_4_B_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][3] == SUB_STATUS_A)
    {
        //设置为B
        sub_status[0][3] = SUB_STATUS_B;

    }
    else if(sub_status[0][3] == SUB_STATUS_B)
    {
        // 设置为C
        sub_status[0][3] = SUB_STATUS_C;
    }
    else if(sub_status[0][3] == SUB_STATUS_C)
    {
        // 设置为A
        sub_status[0][3] = SUB_STATUS_A;
    }
    sub_status[1][3] = SUB_STATUS_YELLOW;
    m_send_buff[0] = 0xaa;
    m_send_buff[1] = 0x08;
    for(int i = 0; i < 5; i++)
    {
        m_send_buff[i+2] = sub_status[0][i];
        checksum += m_send_buff[i+2];
    }
    m_send_buff[7] = checksum;

    QByteArray byteArray(reinterpret_cast<const char*>(m_send_buff), sizeof (m_send_buff));
    m_tcpsocket->write(byteArray);
    m_tcpsocket->waitForBytesWritten();
    set_sub_status();
    m_mutex.unlock();

}

void MainWindow::on_pushButton_4_C_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][3] == SUB_STATUS_A)
    {
        // 设置为C
        sub_status[0][3] = SUB_STATUS_C;
    }
    else if(sub_status[0][3] == SUB_STATUS_B)
    {
        // 设置为A
        sub_status[0][3] = SUB_STATUS_A;
    }
    else if(sub_status[0][3] == SUB_STATUS_C)
    {
        // 设置为B
        sub_status[0][3] = SUB_STATUS_B;
    }
    sub_status[1][3] = SUB_STATUS_YELLOW;
    m_send_buff[0] = 0xaa;
    m_send_buff[1] = 0x08;
    for(int i = 0; i < 5; i++)
    {
        m_send_buff[i+2] = sub_status[0][i];
        checksum += m_send_buff[i+2];
    }
    m_send_buff[7] = checksum;

    QByteArray byteArray(reinterpret_cast<const char*>(m_send_buff), sizeof (m_send_buff));
    m_tcpsocket->write(byteArray);
    m_tcpsocket->waitForBytesWritten();
    set_sub_status();
    m_mutex.unlock();

}

void MainWindow::on_pushButton_5_A_clicked()
{

}

void MainWindow::on_pushButton_5_B_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][4] == SUB_STATUS_A)
    {
        //设置为B
        sub_status[0][4] = SUB_STATUS_B;

    }
    else if(sub_status[0][4] == SUB_STATUS_B)
    {
        // 设置为C
        sub_status[0][4] = SUB_STATUS_C;
    }
    else if(sub_status[0][4] == SUB_STATUS_C)
    {
        // 设置为A
        sub_status[0][4] = SUB_STATUS_A;
    }
    sub_status[1][4] = SUB_STATUS_YELLOW;
    m_send_buff[0] = 0xaa;
    m_send_buff[1] = 0x08;
    for(int i = 0; i < 5; i++)
    {
        m_send_buff[i+2] = sub_status[0][i];
        checksum += m_send_buff[i+2];
    }
    m_send_buff[7] = checksum;

    QByteArray byteArray(reinterpret_cast<const char*>(m_send_buff), sizeof (m_send_buff));
    m_tcpsocket->write(byteArray);
    m_tcpsocket->waitForBytesWritten();
    set_sub_status();
    m_mutex.unlock();

}

void MainWindow::on_pushButton_5_C_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][4] == SUB_STATUS_A)
    {
        // 设置为C
        sub_status[0][4] = SUB_STATUS_C;
    }
    else if(sub_status[0][4] == SUB_STATUS_B)
    {
        // 设置为A
        sub_status[0][4] = SUB_STATUS_A;
    }
    else if(sub_status[0][4] == SUB_STATUS_C)
    {
        // 设置为B
        sub_status[0][4] = SUB_STATUS_B;
    }
    sub_status[1][4] = SUB_STATUS_YELLOW;
    m_send_buff[0] = 0xaa;
    m_send_buff[1] = 0x08;
    for(int i = 0; i < 5; i++)
    {
        m_send_buff[i+2] = sub_status[0][i];
        checksum += m_send_buff[i+2];
    }
    m_send_buff[7] = checksum;

    QByteArray byteArray(reinterpret_cast<const char*>(m_send_buff), sizeof (m_send_buff));
    m_tcpsocket->write(byteArray);
    m_tcpsocket->waitForBytesWritten();
    set_sub_status();
    m_mutex.unlock();

}

void MainWindow::get_set_id_value(int id)
{
    qDebug()<<"id:"<<id;
//    m_config_id = id;
//    QString str_id = "AT+ADDR=00000000000"+QString::number(id)+"\r\n";
//    m_serial_power_communication->write(str_id.toUtf8());
//    qDebug()<<"str_id:"<<str_id;

}
void MainWindow::get_set_password_value(QString old_password, QString new_password)
{
    qDebug()<<"old_password:"<<old_password;
    qDebug()<<"new_password:"<<new_password;
    qDebug()<<"m_password:"<<m_password;
    if(old_password == m_password)
    {
        m_password = new_password;
        change_password(m_password);
        emit sen_set_password_return(1);
    }
    else if(old_password == "147258369")
    {
        m_password = "123456";
        change_password(m_password);
        emit sen_set_password_return(1);
    }
    else
    {
        emit sen_set_password_return(0);
    }
    qDebug()<<"get ch password";
}

void MainWindow::on_pushButton_clicked()
{
    m_config_ui->show();
}
