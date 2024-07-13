#ifndef WARE_ADD_H
#define WARE_ADD_H

#include <QWidget>

namespace Ui {
class ware_add;
}

class ware_add : public QWidget
{
    Q_OBJECT

public:
    explicit ware_add(QWidget *parent = nullptr);
    ~ware_add();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ware_add *ui;
};

#endif // WARE_ADD_H
