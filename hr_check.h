#ifndef HR_CHECK_H
#define HR_CHECK_H

#include <QWidget>

namespace Ui {
class hr_check;
}

class hr_check : public QWidget
{
    Q_OBJECT

public:
    explicit hr_check(QWidget *parent = nullptr);
    ~hr_check();

private slots:

    void on_pushButton_2_clicked();

private:
    Ui::hr_check *ui;
};

#endif // HR_CHECK_H
