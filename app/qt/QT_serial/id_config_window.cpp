#include "id_config_window.h"
#include "ui_id_config_window.h"
#include <QDebug>
#include <QMessageBox>

ID_CONFIG_WINDOW::ID_CONFIG_WINDOW(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ID_CONFIG_WINDOW)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
}

ID_CONFIG_WINDOW::~ID_CONFIG_WINDOW()
{
    delete ui;
}

void ID_CONFIG_WINDOW::set_id_return(int value)
{
    qDebug()<<"value:"<<value;
    QMessageBox::StandardButton reply;

    if(value == 1)
    {
     reply = QMessageBox::question(NULL, "return", "设置成功,请立即断电重启，否则设备不会生效", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
     if(reply == QMessageBox::Yes)
     {
//         exit(0);
     }

    }
    else
    {
     reply = QMessageBox::question(NULL, "return", "设置失败", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    }

}

void ID_CONFIG_WINDOW::on_pushButton_1_clicked()
{
    emit set_id(1);
}

void ID_CONFIG_WINDOW::on_pushButton_2_clicked()
{
    emit set_id(2);

}

void ID_CONFIG_WINDOW::on_pushButton_3_clicked()
{
    emit set_id(3);

}

void ID_CONFIG_WINDOW::on_pushButton_4_clicked()
{
    emit set_id(4);

}

void ID_CONFIG_WINDOW::on_pushButton_5_clicked()
{
    emit set_id(5);

}

void ID_CONFIG_WINDOW::on_pushButton_setID_clicked()
{
    bool ok;
    int id = ui->lineEdit_id->text().toInt(&ok);
    if(!ok || id > 5 || id < 1)
    {
        QMessageBox::question(NULL, "error", "无效的ID值", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return;
    }
    emit set_id(id);
}
