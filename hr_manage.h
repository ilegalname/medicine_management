#ifndef HR_MANAGE_H
#define HR_MANAGE_H

#include <QWidget>

namespace Ui {
class hr_manage;
}

class hr_manage : public QWidget
{
    Q_OBJECT

public:
    explicit hr_manage(QWidget *parent = nullptr);
    ~hr_manage();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::hr_manage *ui;
};

#endif // HR_MANAGE_H
