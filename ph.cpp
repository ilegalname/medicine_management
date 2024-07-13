#include "ph.h"
#include "ui_ph.h"
#include "ph_check.h"
#include "sell_me.h"
#include "apply_m.h"
#include "check_apply.h"
//string current_id;

ph::ph(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ph)
{
    ui->setupUi(this);
}

ph::~ph()
{
    delete ui;
}

void ph::on_pushButton_3_clicked()         //申请补货
{
    apply_m *apm = new apply_m;
    apm->show();

}

void ph::on_pushButton_2_clicked()          //卖药
{

    sell_me *sell = new sell_me;
    sell->show();
}

void ph::on_pushButton_clicked()            //检查库存
{

    ph_check *checkm = new ph_check;
    checkm->show();
}

void ph::on_pushButton_4_clicked()
{
    check_apply *ca = new check_apply;
    ca->show();
}
