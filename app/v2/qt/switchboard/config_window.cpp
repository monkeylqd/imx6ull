#include "config_window.h"
#include "ui_config_window.h"
#include <QMessageBox>

config_window::config_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::config_window)
{
    ui->setupUi(this);
    ui->lineEdit_old_password->setEchoMode(QLineEdit::Password);
    ui->lineEdit_new_password1->setEchoMode(QLineEdit::Password);
    ui->lineEdit_new_password2->setEchoMode(QLineEdit::Password);
}

config_window::~config_window()
{
    delete ui;
}

void config_window::get_set_id_return(int value)
{
    if(value == 1)
    {
        QMessageBox::information(this, "标题", "设备ID设置成功，请给设备重新上下电。");
    }
    else
    {
        QMessageBox::information(this, "标题", "设备ID设置失败，请重试。");
    }
}

void config_window::get_set_password_return(int value)
{
    if(value == 1)
    {
        QMessageBox::information(this, "标题", "密码更改成功，请妥善保存密码。");
    }
    else
    {
        QMessageBox::information(this, "标题", "密码更改失败，请重试。");
    }
    qDebug()<<"get_set_password_return";
}


void config_window::on_pushButton_ch_password_clicked()
{
    QString old_password = ui->lineEdit_old_password->text();
    QString new_password1 = ui->lineEdit_new_password1->text();
    QString new_password2 = ui->lineEdit_new_password2->text();
    if(new_password1 != new_password2)
    {
        QMessageBox::information(this, "标题", "两次新密码不一致。");
        qDebug()<<"两次新密码不一致。";
    }
    else
    {
        emit sen_set_password_value(old_password, new_password1);
        qDebug()<<"send ch password";
    }
}

void config_window::on_pushButton_quit_clicked()
{
    this->hide();

}

void config_window::on_pushButton_ch_id_clicked()
{
    emit sen_set_id_value(ui->lineEdit_device_id->text().toInt());
}
