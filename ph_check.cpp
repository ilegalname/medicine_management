#include "ph_check.h"
#include "ui_ph_check.h"
#include "mainwindow.h"
#include "ph.h"
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

SQLRETURN rett = NULL;
SQLHENV henvt = NULL;
SQLHDBC hdbct = NULL;
SQLHSTMT hstmtt = NULL;

ph_check::ph_check(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ph_check)
{
    ui->setupUi(this);
}

ph_check::~ph_check()
{
    delete ui;
}

string qstr_str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}

void connect_db1()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henvt);//申请环境
    SQLSetEnvAttr(henvt, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henvt, &hdbct);//申请数据库连接
    rett = SQLConnect(hdbct, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((rett == SQL_SUCCESS) || (rett == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}

void free1()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmtt);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbct);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henvt);//释放环境
}

void ph_check::on_pushButton_clicked()         //查询单个药品
{
    connect_db1();
    ui->textBrowser->clear();
    QString M_id = ui->lineEdit->text();
    string mid = qstr_str(M_id);
    rett = SQLAllocHandle(SQL_HANDLE_STMT, hdbct, &hstmtt);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT Mid, Mname, Mnum, Mpriceout FROM Mediout where Mid='";
    string str3 = str2 + mid + "'";
    rett = SQLExecDirect(hstmtt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    rett = SQLExecDirect(hstmtt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (rett == SQL_SUCCESS || rett == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        //cout << "Medicine_id" << "    " << "Medicine_name" << "    " << "Medicine_num" << "     " << "Medicine_price" << endl;
        //string sta = "Medicine_id  Medicine_name  Medicine_num  Medicine_price";
        //ui->textBrowser->insertPlainText(QString::fromLocal8Bit(sta.data())+='\n');
        while (SQLFetch(hstmtt) != SQL_NO_DATA)
        {
            SQLGetData(hstmtt, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmtt, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmtt, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmtt, 4, SQL_C_CHAR, str4, 30, &len_str4);
            string str = "  ";
            string str_1((const char*)str1);
            string str_2((const char*)str2);
            string str_3((const char*)str3);
            string str_4((const char*)str4);
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str.data())+=' ');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_1.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_2.data())+='\t \t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_3.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_4.data())+='\n');
            printf("%s\t%s\t%s\t%s\n", str1, str2, str3, str4);
        }
        }
        free1();

}

void ph_check::on_pushButton_2_clicked()
{
    connect_db1();
    ui->textBrowser->clear();
    rett = SQLAllocHandle(SQL_HANDLE_STMT, hdbct, &hstmtt);//申请句柄
        string str1 = "use Medicine";
        string str2 = "SELECT Mid, Mname, Mnum, Mpriceout FROM Mediout";
        rett = SQLExecDirect(hstmtt, (SQLCHAR*)str1.c_str(), SQL_NTS);
        rett = SQLExecDirect(hstmtt, (SQLCHAR*)str2.c_str(), SQL_NTS);
        if (rett == SQL_SUCCESS || rett == SQL_SUCCESS_WITH_INFO)
        {
            SQLCHAR str1[10], str2[12], str3[20], str4[30];
            SQLLEN len_str1, len_str2, len_str3, len_str4;
            while (SQLFetch(hstmtt) != SQL_NO_DATA)
            {
                SQLGetData(hstmtt, 1, SQL_C_CHAR, str1, 10, &len_str1);
                SQLGetData(hstmtt, 2, SQL_C_CHAR, str2, 12, &len_str2);
                SQLGetData(hstmtt, 3, SQL_C_CHAR, str3, 20, &len_str3);
                SQLGetData(hstmtt, 4, SQL_C_CHAR, str4, 30, &len_str4);
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
        free1();
}
