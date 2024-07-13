#include "sell_me.h"
#include "ui_sell_me.h"
#include "mainwindow.h"
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <climits>
#include <set>
#include <tuple>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <QMessageBox>
#include <QApplication>
#include <QWidget>
#include <QString>
#undef UNICODE
#undef _UNICODE
#include <string.h>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <cstring>
using namespace std;
using std::to_string;

int i=0;
int cnt = 0;
int buy_mid[20]={0};
int buy_mnum[20];
int current_num[20];
string mediname[20];

SQLRETURN rets = NULL;
SQLHENV henvs = NULL;
SQLHDBC hdbcs = NULL;
SQLHSTMT hstmts = NULL;

void sold_mid(int mediout_num, string medi_outno, int medisold_num, string medi_outname, string sold_date);

sell_me::sell_me(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sell_me)
{
    ui->setupUi(this);
}

sell_me::~sell_me()
{
    delete ui;
}

string qstrtstr(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}

void connect_db2()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henvs);//申请环境
    SQLSetEnvAttr(henvs, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henvs, &hdbcs);//申请数据库连接
    rets = SQLConnect(hdbcs, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((rets == SQL_SUCCESS) || (rets == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}

void free2()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmts);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbcs);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henvs);//释放环境
}


void sell_me::on_pushButton_clicked()
{
    connect_db2();
    rets = SQLAllocHandle(SQL_HANDLE_STMT, hdbcs, &hstmts);//申请句柄
        string str1 = "use Medicine";
        string str2 = "SELECT Mid, Mname, Mnum, Mpriceout FROM Mediout";
        rets = SQLExecDirect(hstmts, (SQLCHAR*)str1.c_str(), SQL_NTS);
        rets = SQLExecDirect(hstmts, (SQLCHAR*)str2.c_str(), SQL_NTS);
        if (rets == SQL_SUCCESS || rets == SQL_SUCCESS_WITH_INFO)
        {
            SQLCHAR str1[10], str2[12], str3[20], str4[30];
            SQLLEN len_str1, len_str2, len_str3, len_str4;
            string sta = "Medicine_id  Medicine_name  Medicine_num  Medicine_price";
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(sta.data())+='\n');
            while (SQLFetch(hstmts) != SQL_NO_DATA)
            {
                SQLGetData(hstmts, 1, SQL_C_CHAR, str1, 10, &len_str1);
                SQLGetData(hstmts, 2, SQL_C_CHAR, str2, 12, &len_str2);
                SQLGetData(hstmts, 3, SQL_C_CHAR, str3, 20, &len_str3);
                SQLGetData(hstmts, 4, SQL_C_CHAR, str4, 30, &len_str4);
                string str = "  ";
                string str_1((const char*)str1);
                string str_2((const char*)str2);
                string str_3((const char*)str3);
                string str_4((const char*)str4);
                ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str.data())+=' ');
                ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_1.data())+='\t');
                ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_2.data())+='\t');
                ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_3.data())+='\t');
                ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_4.data())+='\n');
                printf("%s\t%s\t%s\t%s\n", str1, str2, str3, str4);
            }
        }
}

void sell_me::on_pushButton_2_clicked()
{

    ui->textBrowser_3->clear();
    int single_price = 0;
    connect_db2();
    QString M_id = ui->lineEdit->text();
    string mid = qstrtstr(M_id);
    QString M_num = ui->lineEdit_2->text();
    string mnum = qstrtstr(M_num);
    rets = SQLAllocHandle(SQL_HANDLE_STMT, hdbcs, &hstmts);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT Mid, Mname, Mpriceout, Mnum FROM Mediout where Mid='";
    string str3 = str2 + mid + "'";
    rets = SQLExecDirect(hstmts, (SQLCHAR*)str1.c_str(), SQL_NTS);
    rets = SQLExecDirect(hstmts, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (rets == SQL_SUCCESS || rets == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        while (SQLFetch(hstmts) != SQL_NO_DATA)
        {
            SQLGetData(hstmts, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmts, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmts, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmts, 4, SQL_C_CHAR, str4, 20, &len_str4);
            //printf("%s\t%s\t%s\t%s\n", str1, str2, str3, str4);
        }
        string str = "  ";
        string str_1((const char*)str1);
        string str_2((const char*)str2);
        string str_3((const char*)str3);
        string str_4((const char*)str4);

        int medicine_num;
        istringstream sss(str_4);
        sss >> medicine_num;
        istringstream ss(str_3);
        ss >> single_price;
        int want_num;
        istringstream s(mnum);
        s >> want_num;
        printf("medicinenum:%d,single:%d,want_num=%d\n",medicine_num,single_price,want_num);
        if(want_num <= medicine_num){
            ui->textBrowser_2->insertPlainText(QString::fromLocal8Bit(str.data())+='\n');
            ui->textBrowser_2->insertPlainText(QString::fromLocal8Bit(str_1.data())+='\t');
            ui->textBrowser_2->insertPlainText(QString::fromLocal8Bit(str_2.data())+='\t');
            ui->textBrowser_2->insertPlainText(QString::fromLocal8Bit(mnum.data())+='\t');
            ui->textBrowser_2->insertPlainText(QString::fromLocal8Bit(str_3.data())+='\n');
            cnt = cnt + single_price * want_num;
            printf("cnt:%d",cnt);
            string all = to_string(cnt);
            ui->textBrowser_3->insertPlainText(QString::fromLocal8Bit(all.data()));
            istringstream cc(str_1);
            int bi;
            cc >> bi;
            buy_mid[i] = bi;
            buy_mnum[i] = want_num;
            current_num[i] = medicine_num;
            mediname[i]=str_2;
            printf("cm:%d,bid:%d,want_num=%d,%c/n",current_num[i],buy_mid[i],buy_mnum[i],mediname[i]);
            i++;
        }
        else{
            QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("not enough medicines"));
        }
    }

}

void sell_me::on_pushButton_3_clicked()
{
    connect_db2();
    i=0;
    cnt=0;
    QString M_time = ui->lineEdit_3->text();
    string sold_date = qstrtstr(M_time);
    for(int j=0;buy_mid[j]!=0;j++){
        printf("cm:%d,bid:%d,want_num=%d/n",current_num[j],buy_mid[j],buy_mnum[j]);
        sold_mid(current_num[j],to_string(buy_mid[j]),buy_mnum[j],mediname[j],sold_date);
    }
}

void sold_mid(int mediout_num, string medi_outno, int medisold_num, string medi_outname, string sold_date) {
    if (mediout_num >= medisold_num) {
        rets = SQLAllocHandle(SQL_HANDLE_STMT, hdbcs, &hstmts);//申请句柄
        //更新药房药品数量
        int num_update;
        num_update = mediout_num - medisold_num;
        string str1 = "use Medicine";
        string str2 = "UPDATE Mediout SET Mnum = " + to_string(num_update) + " Where Mediout.Mid = " + medi_outno;

        rets = SQLExecDirect(hstmts, (SQLCHAR*)str1.c_str(), SQL_NTS);
        rets = SQLExecDirect(hstmts, (SQLCHAR*)str2.c_str(), SQL_NTS);
        free2();

        //更新出售记录单
        rets = SQLAllocHandle(SQL_HANDLE_STMT, hdbcs, &hstmts);
        cout << "请依次输入工号、售出时间" << endl;

        string str3 = "use Medicine";
        string str4 = "INSERT INTO Medi_sold (Mid,Mname,Mnumout,Mworkerout,Mtimeout) Values('"+ medi_outno + "','" + medi_outname + "','" + to_string(medisold_num) + "','" + to_string(current_id) + "','" + sold_date + "')";
        rets = SQLExecDirect(hstmts, (SQLCHAR*)str3.c_str(), SQL_NTS);
        rets = SQLExecDirect(hstmts, (SQLCHAR*)str4.c_str(), SQL_NTS);
    }
    else {
        cout << "数量不足，请重新填写购买数量";
    }
    free2();
}
