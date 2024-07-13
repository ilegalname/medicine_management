#ifndef CHECK_APPLY_H
#define CHECK_APPLY_H

#include <QWidget>

namespace Ui {
class check_apply;
}

class check_apply : public QWidget
{
    Q_OBJECT

public:
    explicit check_apply(QWidget *parent = nullptr);
    ~check_apply();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::check_apply *ui;
};

#endif // CHECK_APPLY_H
