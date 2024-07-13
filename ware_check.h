#ifndef WARE_CHECK_H
#define WARE_CHECK_H

#include <QWidget>

namespace Ui {
class ware_check;
}

class ware_check : public QWidget
{
    Q_OBJECT

public:
    explicit ware_check(QWidget *parent = nullptr);
    ~ware_check();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ware_check *ui;
};

#endif // WARE_CHECK_H
