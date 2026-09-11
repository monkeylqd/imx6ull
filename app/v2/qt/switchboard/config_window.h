#ifndef CONFIG_WINDOW_H
#define CONFIG_WINDOW_H

#include <QWidget>
#include <QString>
#include <QDebug>

namespace Ui {
class config_window;
}

class config_window : public QWidget
{
    Q_OBJECT

public:
    explicit config_window(QWidget *parent = nullptr);
    ~config_window();

signals:
    void sen_set_id_value(int value);
    void sen_set_password_value(QString old_password, QString new_password);
public slots:
    void get_set_id_return(int value);
    void get_set_password_return(int value);
private slots:


    void on_pushButton_ch_password_clicked();

    void on_pushButton_quit_clicked();

    void on_pushButton_ch_id_clicked();

private:
    Ui::config_window *ui;
};

#endif // CONFIG_WINDOW_H
