#ifndef PH_H
#define PH_H

#include <QWidget>

namespace Ui {
class ph;
}

class ph : public QWidget
{
    Q_OBJECT

public:
    explicit ph(QWidget *parent = nullptr);
    ~ph();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::ph *ui;
};

#endif // PH_H
