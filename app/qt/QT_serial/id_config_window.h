#ifndef ID_CONFIG_WINDOW_H
#define ID_CONFIG_WINDOW_H

#include <QWidget>

namespace Ui {
class ID_CONFIG_WINDOW;
}

class ID_CONFIG_WINDOW : public QWidget
{
    Q_OBJECT

public:
    explicit ID_CONFIG_WINDOW(QWidget *parent = nullptr);
    ~ID_CONFIG_WINDOW();

signals:
    void set_id(int id);


public slots:
    void set_id_return(int value);

private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::ID_CONFIG_WINDOW *ui;
};

#endif // ID_CONFIG_WINDOW_H
