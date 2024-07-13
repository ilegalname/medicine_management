#include "ware_apply.h"
#include "ui_ware_apply.h"
#include "mainwindow.h"
#include "ware_check.h"
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
using namespace std;
using std::to_string;

SQLRETURN retwwa = NULL;
SQLHENV henvwwa = NULL;
SQLHDBC hdbcwwa = NULL;
SQLHSTMT hstmtwwa = NULL;

string qstr12str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}

void free12()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmtwwa);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbcwwa);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henvwwa);//释放环境
}

void connect_db12()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henvwwa);//申请环境
    SQLSetEnvAttr(henvwwa, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henvwwa, &hdbcwwa);//申请数据库连接
    retwwa = SQLConnect(hdbcwwa, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((retwwa == SQL_SUCCESS) || (retwwa == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}


ware_apply::ware_apply(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ware_apply)
{
    ui->setupUi(this);
}

ware_apply::~ware_apply()
{
    delete ui;
}

void ware_apply::on_pushButton_clicked()
{
    ui->textBrowser->clear();
    connect_db12();
    retwwa = SQLAllocHandle(SQL_HANDLE_STMT, hdbcwwa, &hstmtwwa);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Medi_apply";
    retwwa = SQLExecDirect(hstmtwwa, (SQLCHAR*)str1.c_str(), SQL_NTS);
    retwwa = SQLExecDirect(hstmtwwa, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (retwwa == SQL_SUCCESS || retwwa == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[20];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        while (SQLFetch(hstmtwwa) != SQL_NO_DATA)
        {
            SQLGetData(hstmtwwa, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmtwwa, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmtwwa, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmtwwa, 4, SQL_C_CHAR, str4, 20, &len_str4);
            string str_1((const char*)str1);
            string str_2((const char*)str2);
            string str_3((const char*)str3);
            string str_4((const char*)str4);
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_1.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_2.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_3.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_4.data())+='\n');
            }
        }
        free12();
}


int check_ware(string ap_id){
    connect_db12();
    string mcount;
    retwwa = SQLAllocHandle(SQL_HANDLE_STMT, hdbcwwa, &hstmtwwa);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Medi where Mid='";
    string str3 = str2 + ap_id + "'";
    retwwa = SQLExecDirect(hstmtwwa, (SQLCHAR*)str1.c_str(), SQL_NTS);
    retwwa = SQLExecDirect(hstmtwwa, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (retwwa == SQL_SUCCESS || retwwa == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        while (SQLFetch(hstmtwwa) != SQL_NO_DATA)
        {
            SQLGetData(hstmtwwa, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmtwwa, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmtwwa, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmtwwa, 4, SQL_C_CHAR, str4, 30, &len_str4);
            string str_1((const char*)str1);
            string str_2((const char*)str2);
            string str_3((const char*)str3);
            string str_4((const char*)str4);
            mcount = str_3;
        }
        }
    free12();
    int mcnt;
    istringstream sss(mcount);
    sss >> mcnt;
    return mcnt;
}

void ware_apply::on_pushButton_2_clicked()
{
    connect_db12();
    int num_apply;
    string id_apply;
    QString A_id = ui->lineEdit->text();
    string aid = qstr12str(A_id);
    retwwa = SQLAllocHandle(SQL_HANDLE_STMT, hdbcwwa, &hstmtwwa);//申请句柄
    //查看订单需要多少药
    string str1 = "use Medicine";
    string str2 = "SELECT Aid, Mid, Mnum FROM Medi_apply where Aid='";
    string str3 = str2 + aid + "'";
    retwwa = SQLExecDirect(hstmtwwa, (SQLCHAR*)str1.c_str(), SQL_NTS);
    retwwa = SQLExecDirect(hstmtwwa, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (retwwa == SQL_SUCCESS || retwwa == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[12];
        SQLLEN len_str1, len_str2, len_str3;
        while (SQLFetch(hstmtwwa) != SQL_NO_DATA)
        {
            SQLGetData(hstmtwwa, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmtwwa, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmtwwa, 3, SQL_C_CHAR, str3, 12, &len_str3);
            string str_1((const char*)str1);
            string str_2((const char*)str2);
            string str_3((const char*)str3);
            istringstream sss(str_3);
            sss >> num_apply;
            id_apply = str_2;
        }
    }
    free12();
    int number = check_ware(id_apply);
    printf("number:%d,apply:%d\n",number,num_apply);

    //更新订单状态
    string ap_state;
    if(ui->comboBox->currentIndex()==0){
        if(number>=num_apply){
            retwwa = SQLAllocHandle(SQL_HANDLE_STMT, hdbcwwa, &hstmtwwa);//申请句柄
            ap_state = "unchecked";
            string str8 = "use Medicine";
            string str9 = "UPDATE Medi_apply SET Mstate = '" + ap_state + "' where Medi_apply.Aid =" + aid;
            retwwa = SQLExecDirect(hstmtwwa, (SQLCHAR*)str8.c_str(), SQL_NTS);
            retwwa = SQLExecDirect(hstmtwwa, (SQLCHAR*)str9.c_str(), SQL_NTS);
            free12();
            //更新仓库药品数量
            retwwa = SQLAllocHandle(SQL_HANDLE_STMT, hdbcwwa, &hstmtwwa);
            int num_update;
            num_update = number - num_apply;
            string str6 = "use Medicine";
            string str7 = "UPDATE Medi SET Mcount = " + to_string(num_update) + " Where Medi.Mid = " + id_apply;
            retwwa = SQLExecDirect(hstmtwwa, (SQLCHAR*)str6.c_str(), SQL_NTS);
            retwwa = SQLExecDirect(hstmtwwa, (SQLCHAR*)str7.c_str(), SQL_NTS);
            free12();
        }
        else{
            QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("not enough medicines, can't finnish"));
        }
    }
    else{
        ap_state = to_string(number);
        retwwa = SQLAllocHandle(SQL_HANDLE_STMT, hdbcwwa, &hstmtwwa);//申请句柄
        string str8 = "use Medicine";
        string str9 = "UPDATE Medi_apply SET Mstate = 'only" + ap_state + "' where Medi_apply.Aid =" + aid;
        retwwa = SQLExecDirect(hstmtwwa, (SQLCHAR*)str8.c_str(), SQL_NTS);
        retwwa = SQLExecDirect(hstmtwwa, (SQLCHAR*)str9.c_str(), SQL_NTS);
        free12();
    }
}

void ware_apply::on_pushButton_3_clicked()
{
    ware_check *wc = new ware_check;
    wc->show();
}
