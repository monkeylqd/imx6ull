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

    m_slave_name_LabelList.append(ui->slave_1_name);
    m_slave_name_LabelList.append(ui->slave_2_name);
    m_slave_name_LabelList.append(ui->slave_3_name);
    m_slave_name_LabelList.append(ui->slave_4_name);
    m_slave_name_LabelList.append(ui->slave_5_name);
    // 添加所有的label
    m_vol_cur_LabelList.append(ui->slave_1_vol_ch1);
    m_vol_cur_LabelList.append(ui->slave_1_vol_ch2);
    m_vol_cur_LabelList.append(ui->slave_1_vol_ch3);
    m_vol_cur_LabelList.append(ui->slave_1_cur_ch1);
    m_vol_cur_LabelList.append(ui->slave_1_cur_ch2);
    m_vol_cur_LabelList.append(ui->slave_1_cur_ch3);
    m_vol_cur_LabelList.append(ui->slave_2_vol_ch1);
    m_vol_cur_LabelList.append(ui->slave_2_vol_ch2);
    m_vol_cur_LabelList.append(ui->slave_2_vol_ch3);
    m_vol_cur_LabelList.append(ui->slave_2_cur_ch1);
    m_vol_cur_LabelList.append(ui->slave_2_cur_ch2);
    m_vol_cur_LabelList.append(ui->slave_2_cur_ch3);
    m_vol_cur_LabelList.append(ui->slave_3_vol_ch1);
    m_vol_cur_LabelList.append(ui->slave_3_vol_ch2);
    m_vol_cur_LabelList.append(ui->slave_3_vol_ch3);
    m_vol_cur_LabelList.append(ui->slave_3_cur_ch1);
    m_vol_cur_LabelList.append(ui->slave_3_cur_ch2);
    m_vol_cur_LabelList.append(ui->slave_3_cur_ch3);
    m_vol_cur_LabelList.append(ui->slave_4_vol_ch1);
    m_vol_cur_LabelList.append(ui->slave_4_vol_ch2);
    m_vol_cur_LabelList.append(ui->slave_4_vol_ch3);
    m_vol_cur_LabelList.append(ui->slave_4_cur_ch1);
    m_vol_cur_LabelList.append(ui->slave_4_cur_ch2);
    m_vol_cur_LabelList.append(ui->slave_4_cur_ch3);
    m_vol_cur_LabelList.append(ui->slave_5_vol_ch1);
    m_vol_cur_LabelList.append(ui->slave_5_vol_ch2);
    m_vol_cur_LabelList.append(ui->slave_5_vol_ch3);
    m_vol_cur_LabelList.append(ui->slave_5_cur_ch1);
    m_vol_cur_LabelList.append(ui->slave_5_cur_ch2);
    m_vol_cur_LabelList.append(ui->slave_5_cur_ch3);

    // 添加所有的button
    m_buttonList.append(ui->slave_1_ctl1);
    m_buttonList.append(ui->slave_1_ctl2);
    m_buttonList.append(ui->slave_1_ctl3);
    m_buttonList.append(ui->slave_1_ctl4);
    m_buttonList.append(ui->slave_1_ctl5);
    m_buttonList.append(ui->slave_1_ctl6);

    m_buttonList.append(ui->slave_2_ctl1);
    m_buttonList.append(ui->slave_2_ctl2);
    m_buttonList.append(ui->slave_2_ctl3);
    m_buttonList.append(ui->slave_2_ctl4);
    m_buttonList.append(ui->slave_2_ctl5);
    m_buttonList.append(ui->slave_2_ctl6);

    m_buttonList.append(ui->slave_3_ctl1);
    m_buttonList.append(ui->slave_3_ctl2);
    m_buttonList.append(ui->slave_3_ctl3);
    m_buttonList.append(ui->slave_3_ctl4);
    m_buttonList.append(ui->slave_3_ctl5);
    m_buttonList.append(ui->slave_3_ctl6);

    m_buttonList.append(ui->slave_4_ctl1);
    m_buttonList.append(ui->slave_4_ctl2);
    m_buttonList.append(ui->slave_4_ctl3);
    m_buttonList.append(ui->slave_4_ctl4);
    m_buttonList.append(ui->slave_4_ctl5);
    m_buttonList.append(ui->slave_4_ctl6);

    m_buttonList.append(ui->slave_5_ctl1);
    m_buttonList.append(ui->slave_5_ctl2);
    m_buttonList.append(ui->slave_5_ctl3);
    m_buttonList.append(ui->slave_5_ctl4);
    m_buttonList.append(ui->slave_5_ctl5);
    m_buttonList.append(ui->slave_5_ctl6);

    // 初始化所有的button都不可用
    for(int i = 0; i <m_buttonList.size(); i++)
    {
        m_buttonList.at(i)->setDisabled(true);
    }
    m_connect_flag = 0;

    // 初始化 m_ctl_value
    memset(m_ctl_value, 0, sizeof(m_ctl_value));

    // 初始化socket server的默认IP和PORT
    ui->IP_LineEdit->setText("47.109.24.25");
    ui->port_LineEdit->setText("33306");
    m_tcpsocket = new QTcpSocket(this);
    connect(m_tcpsocket, SIGNAL(connected()), this, SLOT(connected_slot()));
    connect(m_tcpsocket, SIGNAL(disconnected()), this, SLOT(disconnect_slot()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connect_button_clicked()
{
    if(m_connect_flag == 0)
    {
        if(ui->IP_LineEdit->text().isEmpty() || ui->port_LineEdit->text().isEmpty())
        {
            m_tcpsocket->connectToHost("47.109.24.25", 33306);
        }
        else
        {
            m_tcpsocket->connectToHost(ui->IP_LineEdit->text(), ui->port_LineEdit->text().toUInt());
        }
    }
    else
    {
        qDebug()<<"disconnect";
        m_tcpsocket->close();
    }
}

int MainWindow::button_click(int ch, int index)
{
    // 判断参数是否合理
    if(ch > 5 || index > 6)
    {
        qDebug("param error:ch=%d index=%d\n", ch, index);
        return -1;
    }

    // 判断是否收到有效的CTL数据
    if(((m_ctl_value[ch] >> 7) & 0x1) == 0)
    {
        return 0;
    }

    // 对CTL对应的bit取反
    m_ctl_value[ch] = m_ctl_value[ch] ^ (1<<index);

    // 根据对应的bit设置按钮状态
    if(((m_ctl_value[ch] >> index) & 0x1) == 1)
    {
        m_buttonList.at(ch*6+index)->setText("开");
    }
    else
    {
        m_buttonList.at(ch*6+index)->setText("关");
    }

    // 将CTL最新的状态发送出去
    send_socket_data();
    return 0;
}

/*
// 发送socket data的接口
*/
int MainWindow::send_socket_data()
{
    m_mutex.lock();
    memset(m_send_buff, 0, sizeof (m_send_buff));
    m_send_buff[0] = 0xaa;
    m_send_buff[1] = APP_ID;
    m_send_buff[2] = 0xff;
    m_send_buff[3] = m_ctl_value[0];
    m_send_buff[4] = m_ctl_value[1];
    m_send_buff[5] = m_ctl_value[2];
    m_send_buff[6] = m_ctl_value[3];
    m_send_buff[7] = m_ctl_value[4];
    m_send_buff[8] = 0x55;
    QByteArray byteArray(reinterpret_cast<const char*>(m_send_buff), sizeof (m_send_buff));
    m_tcpsocket->write(byteArray);
    m_tcpsocket->waitForBytesWritten();
    m_mutex.unlock();
#if 0
    m_mutex.lock();
    memset(m_send_buff, 0, sizeof (m_send_buff));
    m_send_buff[0] = 0xaa;  //头
    m_send_buff[1] = APP_ID;  //app的ID
    m_send_buff[2] = 0xff;  //预留

    // 分机1的通道1 电压电流
    m_send_buff[3] = (m_vol[0][0]>>8) & 0xff;
    m_send_buff[4] =  m_vol[0][0] & 0xff;
    m_send_buff[5] = (m_cur[0][0]>>8) & 0xff;
    m_send_buff[6] =  m_cur[0][0] & 0xff;

    // 分机1的通道2 电压电流
    m_send_buff[7] = (m_vol[0][1]>>8) & 0xff;
    m_send_buff[8] =  m_vol[0][1] & 0xff;
    m_send_buff[9] = (m_cur[0][1]>>8) & 0xff;
    m_send_buff[10] =  m_cur[0][1] & 0xff;

    // 分机1的通道3 电压电流
    m_send_buff[11] = (m_vol[0][2]>>8) & 0xff;
    m_send_buff[12] =  m_vol[0][2] & 0xff;
    m_send_buff[13] = (m_cur[0][2]>>8) & 0xff;
    m_send_buff[14] =  m_cur[0][2] & 0xff;

    // 分机2的通道1 电压电流
    m_send_buff[15] = (m_vol[1][0]>>8) & 0xff;
    m_send_buff[16] =  m_vol[1][0] & 0xff;
    m_send_buff[17] = (m_cur[1][0]>>8) & 0xff;
    m_send_buff[18] =  m_cur[1][0] & 0xff;

    // 分机2通道2 电压电流
    m_send_buff[19] = (m_vol[1][1]>>8) & 0xff;
    m_send_buff[20] =  m_vol[1][1] & 0xff;
    m_send_buff[21] = (m_cur[1][1]>>8) & 0xff;
    m_send_buff[22] =  m_cur[1][1] & 0xff;

    // 分机2的通道3 电压电流
    m_send_buff[23] = (m_vol[1][2]>>8) & 0xff;
    m_send_buff[24] =  m_vol[1][2] & 0xff;
    m_send_buff[25] = (m_cur[1][2]>>8) & 0xff;
    m_send_buff[26] =  m_cur[1][2] & 0xff;

    // 分机3的通道1 电压电流
    m_send_buff[27] = (m_vol[2][0]>>8) & 0xff;
    m_send_buff[28] =  m_vol[2][0] & 0xff;
    m_send_buff[29] = (m_cur[2][0]>>8) & 0xff;
    m_send_buff[30] =  m_cur[2][0] & 0xff;

    // 分机3的通道2 电压电流
    m_send_buff[31] = (m_vol[2][1]>>8) & 0xff;
    m_send_buff[32] =  m_vol[2][1] & 0xff;
    m_send_buff[33] = (m_cur[2][1]>>8) & 0xff;
    m_send_buff[34] =  m_cur[2][1] & 0xff;

    // 分机3的通道3 电压电流
    m_send_buff[35] = (m_vol[2][2]>>8) & 0xff;
    m_send_buff[36] =  m_vol[2][2] & 0xff;
    m_send_buff[37] = (m_cur[2][2]>>8) & 0xff;
    m_send_buff[38] =  m_cur[2][2] & 0xff;

    // 分机4的通道1 电压电流
    m_send_buff[39] = (m_vol[3][0]>>8) & 0xff;
    m_send_buff[40] =  m_vol[3][0] & 0xff;
    m_send_buff[41] = (m_cur[3][0]>>8) & 0xff;
    m_send_buff[42] =  m_cur[3][0] & 0xff;

    // 分机4的通道2 电压电流
    m_send_buff[43] = (m_vol[3][1]>>8) & 0xff;
    m_send_buff[44] =  m_vol[3][1] & 0xff;
    m_send_buff[45] = (m_cur[3][1]>>8) & 0xff;
    m_send_buff[46] =  m_cur[3][1] & 0xff;

    // 分机4的通道3 电压电流
    m_send_buff[47] = (m_vol[3][2]>>8) & 0xff;
    m_send_buff[48] =  m_vol[3][2] & 0xff;
    m_send_buff[49] = (m_cur[3][2]>>8) & 0xff;
    m_send_buff[50] =  m_cur[3][2] & 0xff;

    // 分机5的通道1 电压电流
    m_send_buff[51] = (m_vol[4][0]>>8) & 0xff;
    m_send_buff[52] =  m_vol[4][0] & 0xff;
    m_send_buff[53] = (m_cur[4][0]>>8) & 0xff;
    m_send_buff[54] =  m_cur[4][0] & 0xff;

    // 分机5的通道2 电压电流
    m_send_buff[55] = (m_vol[4][1]>>8) & 0xff;
    m_send_buff[56] =  m_vol[4][1] & 0xff;
    m_send_buff[57] = (m_cur[4][1]>>8) & 0xff;
    m_send_buff[58] =  m_cur[4][1] & 0xff;

    // 分机5的通道3 电压电流
    m_send_buff[59] = (m_vol[4][2]>>8) & 0xff;
    m_send_buff[60] =  m_vol[4][2] & 0xff;
    m_send_buff[61] = (m_cur[4][2]>>8) & 0xff;
    m_send_buff[62] =  m_cur[4][2] & 0xff;


    // 5个分机的ctl值
    m_send_buff[63] = m_ctl_value[0];
    m_send_buff[64] = m_ctl_value[1];
    m_send_buff[65] = m_ctl_value[2];
    m_send_buff[66] = m_ctl_value[3];
    m_send_buff[67] = m_ctl_value[4];

    // 尾
    m_send_buff[68] = 0x55;

    QByteArray byteArray(reinterpret_cast<const char*>(m_send_buff), sizeof (m_send_buff));
    m_tcpsocket->write(byteArray);
    m_tcpsocket->waitForBytesWritten();
    m_mutex.unlock();
#endif
    return 0;
}

void MainWindow::connected_slot()
{
    if(m_connect_flag == 0)
    {
        m_connect_flag = 1;
        connect(m_tcpsocket, SIGNAL(readyRead()), this, SLOT(readyRead_slot()));
        ui->connect_button->setText("disconnect");
        ui->IP_LineEdit->setEnabled(false);
        ui->port_LineEdit->setEnabled(false);
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
        ui->connect_button->setText("connect");
        ui->IP_LineEdit->setEnabled(true);
        ui->port_LineEdit->setEnabled(true);
        for(int i = 0; i <m_buttonList.size(); i++)
        {
            m_buttonList.at(i)->setDisabled(true);
        }
        memset(m_vol, 0, sizeof(m_vol));
        memset(m_cur, 0, sizeof(m_cur));
        for(int i = 0; i < 5; i++)
        {
            m_vol_cur_LabelList.at(i*6+0)->setText(QString::number(m_vol[i][0])+"V");
            m_vol_cur_LabelList.at(i*6+1)->setText(QString::number(m_vol[i][1])+"V");
            m_vol_cur_LabelList.at(i*6+2)->setText(QString::number(m_vol[i][2])+"V");
            m_vol_cur_LabelList.at(i*6+3)->setText(QString::number(m_cur[i][0])+"A");
            m_vol_cur_LabelList.at(i*6+4)->setText(QString::number(m_cur[i][1])+"A");
            m_vol_cur_LabelList.at(i*6+5)->setText(QString::number(m_cur[i][2])+"A");
            m_slave_name_LabelList.at(i)->setStyleSheet("background: transparent;");
        }
    }
}

void MainWindow::readyRead_slot()
{
    cust_delay(5);
    QByteArray data = m_tcpsocket->readAll();
    int rev_id = 0;
    const char* cdata = data.constData();
//    qDebug()<<"rev:"<<m_tcpsocket->readAll();
    if(data.size() == 69)
    {
        rev_id = cdata[1];
        rev_id = rev_id;
        if(rev_id >= 0 && rev_id <= 4)
        {
            m_slave_name_LabelList.at(rev_id)->setStyleSheet("background-color: green;");
            for(int i = 0; i < 6; i++)
            {
                m_buttonList.at(6*rev_id+i)->setDisabled(false);
            }
        }

        for(int i = 0; i < 5; i++)
        {
            m_ctl_value[i] = cdata[i+63];
            if(((m_ctl_value[i] >> 7) & 0x1) == 1)
            {
                m_vol[i][0] = cdata[i*12+0+3]*256 + cdata[i*12+1+3];
                m_cur[i][0] = cdata[i*12+2+3]*256 + cdata[i*12+3+3];
                m_vol[i][1] = cdata[i*12+4+3]*256 + cdata[i*12+5+3];
                m_cur[i][1] = cdata[i*12+6+3]*256 + cdata[i*12+7+3];
                m_vol[i][2] = cdata[i*12+8+3]*256 + cdata[i*12+9+3];
                m_cur[i][2] = cdata[i*12+10+3]*256 + cdata[i*12+11+3];

                for(int j = 0; j < 6; j++)
                {
                    m_buttonList.at(i*6+j)->setDisabled(false);
                    if(((m_ctl_value[i] >> j) & 0x1) == 1)
                    {
                        m_buttonList.at(i*6+j)->setText("开");
                    }
                    else
                    {
                        m_buttonList.at(i*6+j)->setText("关");
                    }
                }
                m_vol_cur_LabelList.at(i*6+0)->setText(QString::number(m_vol[i][0])+"V");
                m_vol_cur_LabelList.at(i*6+1)->setText(QString::number(m_vol[i][1])+"V");
                m_vol_cur_LabelList.at(i*6+2)->setText(QString::number(m_vol[i][2])+"V");
                m_vol_cur_LabelList.at(i*6+3)->setText(QString::number(m_cur[i][0])+"A");
                m_vol_cur_LabelList.at(i*6+4)->setText(QString::number(m_cur[i][1])+"A");
                m_vol_cur_LabelList.at(i*6+5)->setText(QString::number(m_cur[i][2])+"A");
            }
        }
    }
}

void MainWindow::on_slave_1_ctl1_clicked()
{
    button_click(0, 0);
}

void MainWindow::on_slave_1_ctl2_clicked()
{
    button_click(0, 1);

}

void MainWindow::on_slave_1_ctl3_clicked()
{
    button_click(0, 2);

}

void MainWindow::on_slave_1_ctl4_clicked()
{
    button_click(0, 3);

}

void MainWindow::on_slave_1_ctl5_clicked()
{
    button_click(0, 4);

}

void MainWindow::on_slave_1_ctl6_clicked()
{
    button_click(0, 5);

}

void MainWindow::on_slave_2_ctl1_clicked()
{
    button_click(1, 0);

}

void MainWindow::on_slave_2_ctl2_clicked()
{
    button_click(1, 1);

}

void MainWindow::on_slave_2_ctl3_clicked()
{
    button_click(1, 2);

}

void MainWindow::on_slave_2_ctl4_clicked()
{
    button_click(1, 3);

}

void MainWindow::on_slave_2_ctl5_clicked()
{
    button_click(1, 4);

}

void MainWindow::on_slave_2_ctl6_clicked()
{
    button_click(1, 5);

}

void MainWindow::on_slave_3_ctl1_clicked()
{
    button_click(2, 0);

}

void MainWindow::on_slave_3_ctl2_clicked()
{
    button_click(2, 1);

}

void MainWindow::on_slave_3_ctl3_clicked()
{
    button_click(2, 2);

}

void MainWindow::on_slave_3_ctl4_clicked()
{
    button_click(2, 3);

}

void MainWindow::on_slave_3_ctl5_clicked()
{
    button_click(2, 4);

}

void MainWindow::on_slave_3_ctl6_clicked()
{
    button_click(2, 5);

}

void MainWindow::on_slave_4_ctl1_clicked()
{
    button_click(3, 0);

}

void MainWindow::on_slave_4_ctl2_clicked()
{
    button_click(3, 1);

}

void MainWindow::on_slave_4_ctl3_clicked()
{
    button_click(3, 2);

}

void MainWindow::on_slave_4_ctl4_clicked()
{
    button_click(3, 3);

}

void MainWindow::on_slave_4_ctl5_clicked()
{
    button_click(3, 4);

}

void MainWindow::on_slave_4_ctl6_clicked()
{
    button_click(3, 5);

}

void MainWindow::on_slave_5_ctl1_clicked()
{
    button_click(4, 0);

}

void MainWindow::on_slave_5_ctl2_clicked()
{
    button_click(4, 1);

}

void MainWindow::on_slave_5_ctl3_clicked()
{
    button_click(4, 2);

}

void MainWindow::on_slave_5_ctl4_clicked()
{
    button_click(4, 3);

}

void MainWindow::on_slave_5_ctl5_clicked()
{
    button_click(4, 4);

}

void MainWindow::on_slave_5_ctl6_clicked()
{
    button_click(4, 5);

}
