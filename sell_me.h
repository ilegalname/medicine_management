#ifndef SELL_ME_H
#define SELL_ME_H

#include <QWidget>

namespace Ui {
class sell_me;
}

class sell_me : public QWidget
{
    Q_OBJECT

public:
    explicit sell_me(QWidget *parent = nullptr);
    ~sell_me();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::sell_me *ui;
};

#endif // SELL_ME_H
