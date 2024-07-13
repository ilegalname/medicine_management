#ifndef WARE_H
#define WARE_H

#include <QWidget>

namespace Ui {
class ware;
}

class ware : public QWidget
{
    Q_OBJECT

public:
    explicit ware(QWidget *parent = nullptr);
    ~ware();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ware *ui;
};

#endif // WARE_H
