#ifndef REFINELIST_H
#define REFINELIST_H

#include <QWidget>

namespace Ui {
class refinelist;
}

class refinelist : public QWidget
{
    Q_OBJECT

public:
    explicit refinelist(QWidget *parent = nullptr);
    ~refinelist();

private slots:
    void on_pushButton_clicked();

private:
    Ui::refinelist *ui;
};

#endif // REFINELIST_H
