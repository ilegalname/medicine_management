#ifndef PH_CHECK_H
#define PH_CHECK_H

#include <QWidget>

namespace Ui {
class ph_check;
}

class ph_check : public QWidget
{
    Q_OBJECT

public:
    explicit ph_check(QWidget *parent = nullptr);
    ~ph_check();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ph_check *ui;
};

#endif // PH_CHECK_H
