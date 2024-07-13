#ifndef WARE_APPLY_H
#define WARE_APPLY_H

#include <QWidget>

namespace Ui {
class ware_apply;
}

class ware_apply : public QWidget
{
    Q_OBJECT

public:
    explicit ware_apply(QWidget *parent = nullptr);
    ~ware_apply();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::ware_apply *ui;
};

#endif // WARE_APPLY_H
