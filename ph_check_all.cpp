#include "ph_check_all.h"
#include "ui_ph_check_all.h"

ph_check_all::ph_check_all(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ph_check_all)
{
    ui->setupUi(this);
}

ph_check_all::~ph_check_all()
{
    delete ui;
}
