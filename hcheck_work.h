#ifndef HCHECK_WORK_H
#define HCHECK_WORK_H

#include <QWidget>

namespace Ui {
class hcheck_work;
}

class hcheck_work : public QWidget
{
    Q_OBJECT

public:
    explicit hcheck_work(QWidget *parent = nullptr);
    ~hcheck_work();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::hcheck_work *ui;
};

#endif // HCHECK_WORK_H
