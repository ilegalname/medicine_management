#include "ware.h"
#include "ui_ware.h"
#include "ware_check.h"
#include "ware_add.h"
#include "ware_apply.h"

ware::ware(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ware)
{
    ui->setupUi(this);
}

ware::~ware()
{
    delete ui;
}

void ware::on_pushButton_clicked()
{
    ware_check *wc = new ware_check;
    wc->show();
}

void ware::on_pushButton_3_clicked()
{
    ware_add *wa = new ware_add;
    wa->show();
}

void ware::on_pushButton_2_clicked()
{
    ware_apply *wap = new ware_apply;
    wap->show();
}
