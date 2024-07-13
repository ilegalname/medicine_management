#ifndef HR_H
#define HR_H

#include <QWidget>

namespace Ui {
class hr;
}

class hr : public QWidget
{
    Q_OBJECT

public:
    explicit hr(QWidget *parent = nullptr);
    ~hr();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::hr *ui;
};

#endif // HR_H
