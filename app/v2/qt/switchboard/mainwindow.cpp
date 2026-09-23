#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QByteArray>

#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QProcess>
#include <QEventLoop>
#include <QRegularExpression>
#include <QTimer>
#include <QFile>

#define SUB_STATUS_A    (1)
#define SUB_STATUS_B    (2)
#define SUB_STATUS_C    (3)

#define SUB_STATUS_GREEN    (1)
#define SUB_STATUS_YELLOW    (2)

#define PASSWORD_PATH ("password.bat")

void cust_delay(int ms) {
    QEventLoop loop;
    QTimer::singleShot(ms, &loop, SLOT(quit()));
    loop.exec();  // 进入一个局部事件循环
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    m_id = -1;
    get_password();
    m_config_ui = new config_window();
    QObject::connect(this, &MainWindow::sen_set_id_return, m_config_ui, &config_window::get_set_id_return, Qt::QueuedConnection);
    QObject::connect(m_config_ui, &config_window::sen_set_id_value, this, &MainWindow::get_set_id_value, Qt::QueuedConnection);


    QObject::connect(this, &MainWindow::sen_set_password_return, m_config_ui, &config_window::get_set_password_return, Qt::QueuedConnection);
    QObject::connect(m_config_ui, &config_window::sen_set_password_value, this, &MainWindow::get_set_password_value, Qt::QueuedConnection);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(m_timer_slot()));
    m_timer->start(1000);
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
    if(ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty())
    {
        m_tcpsocket->connectToHost("47.109.24.25", 33306);
    }
    else
    {
        m_tcpsocket->connectToHost(ui->lineEdit->text(), ui->lineEdit_2->text().toUInt());
    }
    init_list_ui();

    init_serialport();


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
        connect(m_tcpsocket, SIGNAL(readyRead()), this, SLOT(readyRead_slot()));
        ui->pushButton_3->setText("disconnect");
        ui->lineEdit->setEnabled(false);
        ui->lineEdit_2->setEnabled(false);
        for(int i = 0; i <m_buttonList.size(); i++)
        {
            m_buttonList.at(i)->setEnabled(true);
        }
        m_connect_flag = 1;
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
            for(int n = 0; n < 5 && n < (frame_len-2); n++)
            {
                new_sub_status[0][n] = send_buffer[n+2];
                if(sub_status[0][n] != new_sub_status[0][n])
                {
                    sub_status[1][n] = SUB_STATUS_YELLOW;
                    qDebug()<<"get new status m="<<n;
                }
            }
            qDebug()<<"rev app data";
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

void MainWindow::m_timer_slot()
{
    int i = 0;
    if(m_id < 0)
    {
        m_serial_power_communication->write("AT+ADDR?\r\n");
    }

    for(i = 0; i < 6; i++)
    {
        m_vol_cur_LabelList.at(i*6+0)->setText(QString::number(m_vol_cur_value[i][0])+"V");
        m_vol_cur_LabelList.at(i*6+1)->setText(QString::number(m_vol_cur_value[i][1])+"A");
        m_vol_cur_LabelList.at(i*6+2)->setText(QString::number(m_vol_cur_value[i][2])+"V");
        m_vol_cur_LabelList.at(i*6+3)->setText(QString::number(m_vol_cur_value[i][3])+"A");
        m_vol_cur_LabelList.at(i*6+4)->setText(QString::number(m_vol_cur_value[i][4])+"V");
        m_vol_cur_LabelList.at(i*6+5)->setText(QString::number(m_vol_cur_value[i][5])+"A");
        if(i == m_id)
        {
            if(sub_status[1][i] == SUB_STATUS_YELLOW)
            {
                config_tir_ctl(new_sub_status[0][i]);
                qDebug()<<"config_tir_ctl";
            }
        }
        else
        {
            sub_status[0][i] = new_sub_status[0][i];
        }


    }
    set_sub_status();
    m_count++;
    if(m_count >= 5)
    {
        send_board_data();
        send_data_to_app();
        m_count = 0;
    }

    
    m_serial_vol_curr_CH01->write(">>GetVal");
    m_serial_vol_curr_CH02->write(">>GetVal");
    m_serial_vol_curr_CH03->write(">>GetVal");

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
int MainWindow::update_ui()
{
    for(int i = 0; i < 6; i++)
    {
        m_vol_cur_LabelList.at(i*6+0)->setText(QString::number(m_vol_cur_value[i][0])+"V");
        m_vol_cur_LabelList.at(i*6+1)->setText(QString::number(m_vol_cur_value[i][1])+"A");
        m_vol_cur_LabelList.at(i*6+2)->setText(QString::number(m_vol_cur_value[i][2])+"V");
        m_vol_cur_LabelList.at(i*6+3)->setText(QString::number(m_vol_cur_value[i][3])+"A");
        m_vol_cur_LabelList.at(i*6+4)->setText(QString::number(m_vol_cur_value[i][4])+"V");
        m_vol_cur_LabelList.at(i*6+5)->setText(QString::number(m_vol_cur_value[i][5])+"A");
    }
    set_sub_status();

    return 0;
}


// | Vrms:   0.00000V | Irms:   0.00000A | P:   0.0000W | PF: 0.00000 | F:  0.0000Hz | W:   0.0051KW*H |
void MainWindow::read_serial_vol_curr_CH01()
{
   QByteArray read_buff;
   cust_delay(1000);
   read_buff = m_serial_vol_curr_CH01->readAll();
//   qDebug("CH01 read:%s\n", qPrintable(QString(read_buff)));
   parse_vol_cur_data(0, QString(read_buff));

}

void MainWindow::read_serial_vol_curr_CH02()
{
   QByteArray read_buff;
   cust_delay(1000);
   read_buff = m_serial_vol_curr_CH02->readAll();
//   qDebug("CH02 read:%s\n", qPrintable(QString(read_buff)));
   parse_vol_cur_data(1, QString(read_buff));
}

void MainWindow::read_serial_vol_curr_CH03()
{
   QByteArray read_buff;
   cust_delay(1000);
   read_buff = m_serial_vol_curr_CH03->readAll();
//   qDebug("CH03 read:%s\n", qPrintable(QString(read_buff)));
   parse_vol_cur_data(2, QString(read_buff));
}

void MainWindow::read_serial_power_communication()
{
    QByteArray read_buff;
    cust_delay(120);
    read_buff = m_serial_power_communication->readAll();
//    qDebug("COMM read:%s\n", qPrintable(QString(read_buff)));
    parse_comm_data(QString(read_buff));
}

int MainWindow::init_list_ui()
{


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

    m_vol_cur_LabelList.append(ui->label_0_0V);
    m_vol_cur_LabelList.append(ui->label_0_0A);
    m_vol_cur_LabelList.append(ui->label_0_1V);
    m_vol_cur_LabelList.append(ui->label_0_1A);
    m_vol_cur_LabelList.append(ui->label_0_2V);
    m_vol_cur_LabelList.append(ui->label_0_2A);

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
    return 0;
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

    m_serial_power_communication->write("AT+ADDR?\r\n");
}



int MainWindow::cal_index(int cur, int next)
{
    int index = -1;
    if(cur == SUB_STATUS_A && next == SUB_STATUS_B)
    {
        index = 0;
    }
    else if(cur == SUB_STATUS_A && next == SUB_STATUS_C)
    {
        index = 1;
    }
    else if(cur == SUB_STATUS_B && next == SUB_STATUS_A)
    {
        index = 2;
    }
    else if(cur == SUB_STATUS_B && next == SUB_STATUS_C)
    {
        index = 3;
    }
    else if(cur == SUB_STATUS_C && next == SUB_STATUS_A)
    {
        index = 4;
    }
    else if(cur == SUB_STATUS_C && next == SUB_STATUS_B)
    {
        index = 5;
    }
    return index;
}

int MainWindow::config_tir_ctl(int next_status)
{
    int ret = 0;
    int index = 0;
    if(next_status == sub_status[0][m_id] && sub_status[1][m_id] == SUB_STATUS_GREEN)
    {
        return 0;
    }

    index = cal_index(sub_status[0][m_id], next_status);
    if(index < 0)
    {
        qDebug()<<"cal_index error";
        return -1;
    }

    qDebug()<<"index="<<index;
    ret = ctl_gpio_status(index);
    if(ret == 0)
    {
        sub_status[1][m_id] = SUB_STATUS_GREEN;
        sub_status[0][m_id] = next_status;
    }
    else
    {
        qDebug()<<"ctl_gpio_status fail.";
    }
    m_count = 5;
}

int MainWindow::ctl_gpio_status(int status_index)
{
    int ret = 0;
    int i = 0;
    int jdq_index = 0;
    int stat = 0;
    int ctl = 0;
    JDQ_CTL_INFO_t info[6][4] = {
        {{3,0,1},{4,1,1},{1,1,2},{2,0,1}},
        {{3,1,2},{4,0,1},{1,1,2},{2,0,1}},
        {{1,0,1},{2,1,1},{0,0,0},{0,0,0}},
        {{3,1,2},{4,0,1},{0,0,0},{0,0,0}},
        {{1,0,1},{2,1,1},{0,0,0},{0,0,0}},
        {{3,0,1},{4,1,1},{0,0,0},{0,0,0}}
    };

    for(i = 0; i < 4; i++)
    {
        jdq_index = info[status_index][i].index;
        if(jdq_index == 0)
        {
            break;
        }
        jdq_index = jdq_index -1;
        stat = info[status_index][i].status;
        ctl = info[status_index][i].ctl;

        ret = get_gpio_value(jdq_index);
        if(ret != stat)
        {
            ret = set_gpio_value(jdq_index, stat);
            if(ret != 0 && ctl == 1)
            {
                break;
            }
            else if(ctl == 2)
            {
                continue;
            }
        }
        else
        {
            ret = 0;
        }



        if(ctl == 1)
        {
            break;
        }
    }

    return ret;
}

/*
0:set success
-1:set fail
*/
int MainWindow::set_gpio_value(int gpio_index, int gpio_value)
{
    int ret = 0;
    int i = 0;
    QProcess process;

    ret = get_gpio_value(gpio_index);
    if(ret == gpio_value)
    {
        return 0;
    }

    for(i = 0; i < 50; i++)
    {
        // start set GPIO
        process.start();
        process.start("bash", QStringList() << "-c" << m_ctl_cmd.at(gpio_index+0*6));
        cust_delay(50);
        process.start("bash", QStringList() << "-c" << m_ctl_cmd.at(gpio_index+1*6));
        process.waitForFinished();
        if(process.exitCode() != 0)
        {
            qDebug() << "Command failed:" << process.readAllStandardError();
        }
        cust_delay(20);

        // read GPIO
        ret = get_gpio_value(gpio_index);
        if(ret == gpio_value)
        {
            return 0;
        }

    }

    qDebug()<<"set GPIO "<<gpio_index<<" fail";
    return -1;
}
/*
-1:return error
0:return GPIO status value
1:return GPIO status value
*/
int MainWindow::get_gpio_value(int gpio_index)
{
    int ret = 0;
    bool ok;
    QProcess process_read;
    process_read.start("bash", QStringList() << "-c" << m_ctl_cmd.at(gpio_index+2*6));
    process_read.waitForFinished();
    if (process_read.exitCode() == 0)
    {
        QString result = process_read.readAllStandardOutput();
        ret = result.toInt(&ok);
        if(ok)
        {
            return ret;
        }
        else
        {
            qDebug()<<"process_read result fail";
        }
    }
    else
    {
        qDebug() << "read Command failed:" << process_read.readAllStandardError();
    }
    return -1;
}

void MainWindow::parse_vol_cur_data(int index_ch, QString inputs)
{

    QString test1 = inputs.remove(' ');
    QString input = test1.split("\r\n").first();
    // 使用"|"分割字符串并去除空项
    QStringList parts = input.split("|", QString::SkipEmptyParts);

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
//            qWarning() << "Invalid part format (missing colon):" << trimmedPart;
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
        // m_CH_value[index_ch][i] = (int)measurements.at(i).value;
        if(i < 2)
        {
            m_vol_cur_value[m_id][index_ch*2+i] = (int)measurements.at(i).value;
        }
    }
    // qDebug()<<m_CH_value[index_ch][0]<<" "<<m_CH_value[index_ch][1]<<" "<<m_CH_value[index_ch][2]<<" "<<m_CH_value[index_ch][3]<<" "<<m_CH_value[index_ch][4]<<" "<<m_CH_value[index_ch][5];
#endif
//    m_vol_cur_LabelList.at(m_id*6 + index_ch*2)->setText(QString::number(m_CH_value[index_ch][0])+"V");
//    m_vol_cur_LabelList.at(m_id*6 + index_ch*2+1)->setText(QString::number(m_CH_value[index_ch][1])+"A");

}

int MainWindow::parse_comm_data(QString str)
{
    bool ok;
    qint64 rev_id = -10;
    qint64 rev_len = 0;
    int ret = 0;
    int vol_cur[6];
    int ctl_buff[5];
    memset(ctl_buff, 0, sizeof(ctl_buff));
//    qDebug()<<"parse_comm_data:"<<str;
    if(str.contains("+ADDR="))
    {
        QStringList parts = str.split("=");
        if(parts.size() >= 2)
        {
            m_id = parts[1].toLongLong(&ok, 16);
            if(m_id > 6 || m_id < 1)
            {
                m_id = 1;
            }
            m_id = m_id - 1;
            qDebug()<<"m_id="<<m_id;
            if(m_config_id == m_id)
            {
                emit sen_set_id_return(1);
            }
            else if(m_config_id > 0)
            {
                emit sen_set_id_return(0);
            }
            ui->lineEdit_id->setText(QString::number(m_id));
        }
    }
    else if(str.contains("+RX="))
    {
        QStringList parts = str.split(",");
        if(parts.size() != 4)
        {
            qDebug()<<"error:"<<str;
//            return -1;
        }

        rev_id = parts[1].toLongLong(&ok, 16);
        if(!ok)
        {
            qDebug()<<"change ID to Hex fail.";
        }
        rev_id = rev_id -1;

        // 解析设备发送的数据长度
        rev_len = parts[2].toLongLong(&ok, 16);
        if(!ok)
        {
            qDebug()<<"change Len to Hex fail.";
        }

        QByteArray rev_data = parts[3].toUtf8();
        if(rev_len != rev_data.size())
        {
            qDebug()<<"com rev date len!=23. rev len="<< rev_len;
            return -1;
        }
        if(rev_data.at(rev_data.size()-3) != '5')
        {
            qDebug()<<"com rev data check fail";
        }

        const char* charPtr = rev_data.constData();

        for(int i = 0; i < 6; i++)
        {
            m_vol_cur_value[rev_id][i] = (charPtr[i*3+1]-'0')*100 + (charPtr[i*3+2]-'0')*10 + (charPtr[i*3+3]-'0');
        }

        for(int i = 0; i < 5; i++)
        {
            new_sub_status[0][i] = (charPtr[(i+6)*3+1]-'0')*100 + (charPtr[(i+6)*3+2]-'0')*10 + (charPtr[(i+6)*3+3]-'0');
            if(sub_status[0][i] != new_sub_status[0][i])
            {
                // sub_status[0][i] = new_sub_status[0][i];
                if(rev_id == i)
                {
                    sub_status[1][i] = SUB_STATUS_GREEN;
                    sub_status[0][i] = new_sub_status[0][i];
                }
                else
                {
                    sub_status[1][i] = SUB_STATUS_YELLOW;
                }
            }
            else
            {
                if(rev_id == i)
                {
                    sub_status[1][i] = SUB_STATUS_GREEN;
                }

            }
        }
    }

    if(rev_id+1 == m_id)
    {
        m_count = 5;
    }

    return 0;

}

int MainWindow::send_board_data()
{
    QString str1="AT+TX=FFFFFFFFFFFF,0023,";
    QString str2=str1+"A";
    if(m_id >= 0 && m_id <= 5)
    {
        QString str3=str2+QString("%1").arg(m_vol_cur_value[m_id][0], 3, 10, QChar('0'));
        QString str4=str3+QString("%1").arg(m_vol_cur_value[m_id][1], 3, 10, QChar('0'));
        QString str5=str4+QString("%1").arg(m_vol_cur_value[m_id][2], 3, 10, QChar('0'));
        QString str6=str5+QString("%1").arg(m_vol_cur_value[m_id][3], 3, 10, QChar('0'));
        QString str7=str6+QString("%1").arg(m_vol_cur_value[m_id][4], 3, 10, QChar('0'));
        QString str8=str7+QString("%1").arg(m_vol_cur_value[m_id][5], 3, 10, QChar('0'));
        QString str9=str8+QString("%1").arg(sub_status[0][0], 3, 10, QChar('0'));
        QString str10=str9+QString("%1").arg(sub_status[0][1], 3, 10, QChar('0'));
        QString str11=str10+QString("%1").arg(sub_status[0][2], 3, 10, QChar('0'));
        QString str12=str11+QString("%1").arg(sub_status[0][3], 3, 10, QChar('0'));
        QString str13=str12+QString("%1").arg(sub_status[0][4], 3, 10, QChar('0'));
        QString str14=str13+"5";
        QString str15=str14+"\r\n";
        QByteArray byteArray = str15.toUtf8();
        m_serial_power_communication->write(byteArray);
    }
}

int MainWindow::send_data_to_app()
{
    int i = 0;
    unsigned char data[81];
    unsigned char *ptr = NULL;
    unsigned char check_sum = 0;
    data[0] = 0xaa;
    data[1] = 81;
    ptr = &data[2];
    for(i = 0; i < 6; i++)
    {
        ptr[i*13+0] = (m_vol_cur_value[i][0]>>8) & 0xff;
        ptr[i*13+1] = m_vol_cur_value[i][0] & 0xff;
        ptr[i*13+2] = (m_vol_cur_value[i][1]>>8) & 0xff;
        ptr[i*13+3] = m_vol_cur_value[i][1] & 0xff;
        ptr[i*13+4] = (m_vol_cur_value[i][2]>>8) & 0xff;
        ptr[i*13+5] = m_vol_cur_value[i][2] & 0xff;
        ptr[i*13+6] = (m_vol_cur_value[i][3]>>8) & 0xff;
        ptr[i*13+7] = m_vol_cur_value[i][3] & 0xff;
        ptr[i*13+8] = (m_vol_cur_value[i][4]>>8) & 0xff;
        ptr[i*13+9] = m_vol_cur_value[i][4] & 0xff;
        ptr[i*13+10] = (m_vol_cur_value[i][5]>>8) & 0xff;
        ptr[i*13+11] = m_vol_cur_value[i][5] & 0xff;
        ptr[i*13+12] = sub_status[0][i];
    }
    check_sum = 0;
    for(i = 2; i < 80; i++)
    {
        check_sum += data[i];
    }
    data[80] = check_sum;
    QByteArray buff_data(reinterpret_cast<const char*>(data), sizeof(data));
    m_tcpsocket->write(buff_data);
    m_tcpsocket->waitForBytesWritten();
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
        if (password == m_password)
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
        new_sub_status[0][0] = SUB_STATUS_B;

    }
    else if(sub_status[0][0] == SUB_STATUS_B)
    {
        // 设置为C
        new_sub_status[0][0] = SUB_STATUS_C;
    }
    else if(sub_status[0][0] == SUB_STATUS_C)
    {
        // 设置为A
        new_sub_status[0][0] = SUB_STATUS_A;
    }
    sub_status[1][0] = SUB_STATUS_YELLOW;
    m_count = 5;
    m_mutex.unlock();

}

void MainWindow::on_pushButton_1_C_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][0] == SUB_STATUS_A)
    {
        // 设置为C
        new_sub_status[0][0] = SUB_STATUS_C;
    }
    else if(sub_status[0][0] == SUB_STATUS_B)
    {
        // 设置为A
        new_sub_status[0][0] = SUB_STATUS_A;
    }
    else if(sub_status[0][0] == SUB_STATUS_C)
    {
        // 设置为B
        new_sub_status[0][0] = SUB_STATUS_B;
    }
    sub_status[1][0] = SUB_STATUS_YELLOW;
    m_count = 5;
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
        new_sub_status[0][1] = SUB_STATUS_B;

    }
    else if(sub_status[0][1] == SUB_STATUS_B)
    {
        // 设置为C
        new_sub_status[0][1] = SUB_STATUS_C;
    }
    else if(sub_status[0][1] == SUB_STATUS_C)
    {
        // 设置为A
        new_sub_status[0][1] = SUB_STATUS_A;
    }
    sub_status[1][1] = SUB_STATUS_YELLOW;
    m_count = 5;
    m_mutex.unlock();

}

void MainWindow::on_pushButton_2_C_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][1] == SUB_STATUS_A)
    {
        // 设置为C
        new_sub_status[0][1] = SUB_STATUS_C;
    }
    else if(sub_status[0][1] == SUB_STATUS_B)
    {
        // 设置为A
        new_sub_status[0][1] = SUB_STATUS_A;
    }
    else if(sub_status[0][1] == SUB_STATUS_C)
    {
        // 设置为B
        new_sub_status[0][1] = SUB_STATUS_B;
    }
    sub_status[1][1] = SUB_STATUS_YELLOW;
    m_count = 5;
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
        new_sub_status[0][2] = SUB_STATUS_B;

    }
    else if(sub_status[0][2] == SUB_STATUS_B)
    {
        // 设置为C
        new_sub_status[0][2] = SUB_STATUS_C;
    }
    else if(sub_status[0][2] == SUB_STATUS_C)
    {
        // 设置为A
        new_sub_status[0][2] = SUB_STATUS_A;
    }
    sub_status[1][2] = SUB_STATUS_YELLOW;
    m_count = 5;
    m_mutex.unlock();

}

void MainWindow::on_pushButton_3_C_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][2] == SUB_STATUS_A)
    {
        // 设置为C
        new_sub_status[0][2] = SUB_STATUS_C;
    }
    else if(sub_status[0][2] == SUB_STATUS_B)
    {
        // 设置为A
        new_sub_status[0][2] = SUB_STATUS_A;
    }
    else if(sub_status[0][2] == SUB_STATUS_C)
    {
        // 设置为B
        new_sub_status[0][2] = SUB_STATUS_B;
    }
    sub_status[1][2] = SUB_STATUS_YELLOW;
    m_count = 5;
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
        new_sub_status[0][3] = SUB_STATUS_B;

    }
    else if(sub_status[0][3] == SUB_STATUS_B)
    {
        // 设置为C
        new_sub_status[0][3] = SUB_STATUS_C;
    }
    else if(sub_status[0][3] == SUB_STATUS_C)
    {
        // 设置为A
        new_sub_status[0][3] = SUB_STATUS_A;
    }
    sub_status[1][3] = SUB_STATUS_YELLOW;
    m_count = 5;
    m_mutex.unlock();

}

void MainWindow::on_pushButton_4_C_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][3] == SUB_STATUS_A)
    {
        // 设置为C
        new_sub_status[0][3] = SUB_STATUS_C;
    }
    else if(sub_status[0][3] == SUB_STATUS_B)
    {
        // 设置为A
        new_sub_status[0][3] = SUB_STATUS_A;
    }
    else if(sub_status[0][3] == SUB_STATUS_C)
    {
        // 设置为B
        new_sub_status[0][3] = SUB_STATUS_B;
    }
    sub_status[1][3] = SUB_STATUS_YELLOW;
    m_count = 5;
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
        new_sub_status[0][4] = SUB_STATUS_B;

    }
    else if(sub_status[0][4] == SUB_STATUS_B)
    {
        // 设置为C
        new_sub_status[0][4] = SUB_STATUS_C;
    }
    else if(sub_status[0][4] == SUB_STATUS_C)
    {
        // 设置为A
        new_sub_status[0][4] = SUB_STATUS_A;
    }
    sub_status[1][4] = SUB_STATUS_YELLOW;
    m_count = 5;
    m_mutex.unlock();

}

void MainWindow::on_pushButton_5_C_clicked()
{
    unsigned char checksum = 0;
    m_mutex.lock();
    if(sub_status[0][4] == SUB_STATUS_A)
    {
        // 设置为C
        new_sub_status[0][4] = SUB_STATUS_C;
    }
    else if(sub_status[0][4] == SUB_STATUS_B)
    {
        // 设置为A
        new_sub_status[0][4] = SUB_STATUS_A;
    }
    else if(sub_status[0][4] == SUB_STATUS_C)
    {
        // 设置为B
        new_sub_status[0][4] = SUB_STATUS_B;
    }
    sub_status[1][4] = SUB_STATUS_YELLOW;    
    m_count = 5;
    m_mutex.unlock();

}

void MainWindow::get_set_id_value(int id)
{
    qDebug()<<"id:"<<id;
    if(m_id == id)
    {
        emit sen_set_id_return(2);
        return;
    }
    m_config_id = id;
    QString str_id = "AT+ADDR=00000000000"+QString::number(id)+"\r\n";
    m_serial_power_communication->write(str_id.toUtf8());
    qDebug()<<"str_id:"<<str_id;
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
//    m_config_ui->show();
    m_config_ui->showFullScreen();
}
