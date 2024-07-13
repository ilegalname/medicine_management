#include "hcheck_work.h"
#include "ui_hcheck_work.h"
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

SQLRETURN retcc = NULL;
SQLHENV henvcc = NULL;
SQLHDBC hdbccc = NULL;
SQLHSTMT hstmtcc = NULL;

string qstr11str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}

void free11()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmtcc);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbccc);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henvcc);//释放环境
}

void connect_db11()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henvcc);//申请环境
    SQLSetEnvAttr(henvcc, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henvcc, &hdbccc);//申请数据库连接
    retcc = SQLConnect(hdbccc, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((retcc == SQL_SUCCESS) || (retcc == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}

hcheck_work::hcheck_work(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hcheck_work)
{
    ui->setupUi(this);
}

hcheck_work::~hcheck_work()
{
    delete ui;
}

void hcheck_work::on_pushButton_clicked()
{
    connect_db11();
    ui->textBrowser->clear();
    retcc = SQLAllocHandle(SQL_HANDLE_STMT, hdbccc, &hstmtcc);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Medi_sold";
    retcc = SQLExecDirect(hstmtcc, (SQLCHAR*)str1.c_str(), SQL_NTS);
    retcc = SQLExecDirect(hstmtcc, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (retcc == SQL_SUCCESS || retcc == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30], str5[40];
        SQLLEN len_str1, len_str2, len_str3, len_str4, len_str5;
        cout << "药品编号" << "    " << "药品名称" << "    " << "卖出数量" << "     " << "卖出人工号" << "     " << "卖出时间" << endl;
        while (SQLFetch(hstmtcc) != SQL_NO_DATA)
        {
            SQLGetData(hstmtcc, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmtcc, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmtcc, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmtcc, 4, SQL_C_CHAR, str4, 30, &len_str4);
            SQLGetData(hstmtcc, 5, SQL_C_CHAR, str5, 20, &len_str5);
            string str_1((const char*)str1);
            string str_2((const char*)str2);
            string str_3((const char*)str3);
            string str_4((const char*)str4);
            string str_5((const char*)str5);
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_1.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_2.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_3.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_4.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_5.data())+='\n');
        }
    }
    free11();
}

void hcheck_work::on_pushButton_2_clicked()
{
    connect_db11();
    ui->textBrowser_2->clear();
    retcc = SQLAllocHandle(SQL_HANDLE_STMT, hdbccc, &hstmtcc);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Medi_inlist";
    retcc = SQLExecDirect(hstmtcc, (SQLCHAR*)str1.c_str(), SQL_NTS);
    retcc = SQLExecDirect(hstmtcc, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (retcc == SQL_SUCCESS || retcc == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30], str5[40];
        SQLLEN len_str1, len_str2, len_str3, len_str4, len_str5;
        while (SQLFetch(hstmtcc) != SQL_NO_DATA)
        {
            SQLGetData(hstmtcc, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmtcc, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmtcc, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmtcc, 4, SQL_C_CHAR, str4, 30, &len_str4);
            SQLGetData(hstmtcc, 5, SQL_C_CHAR, str5, 20, &len_str5);
            string str_1((const char*)str1);
            string str_2((const char*)str2);
            string str_3((const char*)str3);
            string str_4((const char*)str4);
            string str_5((const char*)str5);
            ui->textBrowser_2->insertPlainText(QString::fromLocal8Bit(str_1.data())+='\t');
            ui->textBrowser_2->insertPlainText(QString::fromLocal8Bit(str_2.data())+='\t');
            ui->textBrowser_2->insertPlainText(QString::fromLocal8Bit(str_3.data())+='\t');
            ui->textBrowser_2->insertPlainText(QString::fromLocal8Bit(str_4.data())+='\t');
            ui->textBrowser_2->insertPlainText(QString::fromLocal8Bit(str_5.data())+='\n');
        }
    }
    free11();
}
