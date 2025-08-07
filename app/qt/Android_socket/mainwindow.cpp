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
    memset(m_ctl_value, 0, sizeof(m_ctl_value));
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
    for(int i = 0; i <m_buttonList.size(); i++)
    {
        m_buttonList.at(i)->setDisabled(true);
    }
    m_connect_flag = 0;
    memset(m_ctl_value, 0, sizeof(m_ctl_value));
    ui->IP_LineEdit->setText("47.109.24.25");
    ui->port_LineEdit->setText("33306");
//    ui->send_button->setEnabled(false);
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

void MainWindow::on_send_button_clicked()
{
//    m_tcpsocket->write(ui->send_lineEdit->text().toUtf8());
}
int MainWindow::button_click(int ch, int index)
{
    if(ch > 5 || index > 6)
    {
        qDebug("param error:ch=%d index=%d\n", ch, index);
        return -1;
    }
    if(((m_ctl_value[ch] >> 7) & 0x1) == 0)
    {
        return 0;
    }
    m_ctl_value[ch] = m_ctl_value[ch] ^ (1<<index);
    if(((m_ctl_value[ch] >> index) & 0x1) == 1)
    {
        m_buttonList.at(ch*6+index)->setText("开");
    }
    else
    {
        m_buttonList.at(ch*6+index)->setText("关");
    }
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
    }
}

void MainWindow::readyRead_slot()
{
    cust_delay(5);
    QByteArray data = m_tcpsocket->readAll();
    int rev_id = 0;
    const char* cdata = data.constData();
//    qDebug()<<"rev:"<<m_tcpsocket->readAll();
    if(data.size() == 68)
    {
        rev_id = cdata[1]*256+cdata[2];
        rev_id = rev_id - 1;
        if(rev_id >= 0 && rev_id <= 4)
        {
            for(int i = 0; i < 6; i++)
            {
                m_buttonList.at(6*rev_id+i)->setDisabled(false);
            }
        }
        for(int i = 0; i < 5; i++)
        {
            m_vol[i][0] = cdata[i*12+0+3]*256 + cdata[i*12+1+3];
            m_cur[i][0] = cdata[i*12+2+3]*256 + cdata[i*12+3+3];
            m_vol[i][1] = cdata[i*12+4+3]*256 + cdata[i*12+5+3];
            m_cur[i][1] = cdata[i*12+5+3]*256 + cdata[i*12+7+3];
            m_vol[i][2] = cdata[i*12+6+3]*256 + cdata[i*12+9+3];
            m_cur[i][2] = cdata[i*12+10+3]*256 + cdata[i*12+11+3];
        }

        for(int i = 0; i < 5; i++)
        {
            m_ctl_value[i] = cdata[i+63];
            if(((m_ctl_value[i] >> 7) & 0x1) == 1)
            {
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
    if((m_ctl_value[0] >> 7) == 0)
    {
        return;
    }
}

void MainWindow::on_slave_1_ctl2_clicked()
{

}

void MainWindow::on_slave_1_ctl3_clicked()
{

}

void MainWindow::on_slave_1_ctl4_clicked()
{

}

void MainWindow::on_slave_1_ctl5_clicked()
{

}

void MainWindow::on_slave_1_ctl6_clicked()
{

}

void MainWindow::on_slave_2_ctl1_clicked()
{

}

void MainWindow::on_slave_2_ctl2_clicked()
{

}

void MainWindow::on_slave_2_ctl3_clicked()
{

}

void MainWindow::on_slave_2_ctl4_clicked()
{

}

void MainWindow::on_slave_2_ctl5_clicked()
{

}

void MainWindow::on_slave_2_ctl6_clicked()
{

}

void MainWindow::on_slave_3_ctl1_clicked()
{

}

void MainWindow::on_slave_3_ctl2_clicked()
{

}

void MainWindow::on_slave_3_ctl3_clicked()
{

}

void MainWindow::on_slave_3_ctl4_clicked()
{

}

void MainWindow::on_slave_3_ctl5_clicked()
{

}

void MainWindow::on_slave_3_ctl6_clicked()
{

}

void MainWindow::on_slave_4_ctl1_clicked()
{

}

void MainWindow::on_slave_4_ctl2_clicked()
{

}

void MainWindow::on_slave_4_ctl3_clicked()
{

}

void MainWindow::on_slave_4_ctl4_clicked()
{

}

void MainWindow::on_slave_4_ctl5_clicked()
{

}

void MainWindow::on_slave_4_ctl6_clicked()
{

}

void MainWindow::on_slave_5_ctl1_clicked()
{

}

void MainWindow::on_slave_5_ctl2_clicked()
{

}

void MainWindow::on_slave_5_ctl3_clicked()
{

}

void MainWindow::on_slave_5_ctl4_clicked()
{

}

void MainWindow::on_slave_5_ctl5_clicked()
{

}

void MainWindow::on_slave_5_ctl6_clicked()
{

}
