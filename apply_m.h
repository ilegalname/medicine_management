#ifndef APPLY_M_H
#define APPLY_M_H

#include <QWidget>

namespace Ui {
class apply_m;
}

class apply_m : public QWidget
{
    Q_OBJECT

public:
    explicit apply_m(QWidget *parent = nullptr);
    ~apply_m();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::apply_m *ui;
};

#endif // APPLY_M_H
