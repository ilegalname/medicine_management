#ifndef INIT_ID_H
#define INIT_ID_H

#include <QWidget>

namespace Ui {
class init_id;
}

class init_id : public QWidget
{
    Q_OBJECT

public:
    explicit init_id(QWidget *parent = nullptr);
    ~init_id();

private slots:
    void on_pushButton_clicked();

private:
    Ui::init_id *ui;
};

#endif // INIT_ID_H
