#include "ware_check.h"
#include "ui_ware_check.h"
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

SQLRETURN retw = NULL;
SQLHENV henvw = NULL;
SQLHDBC hdbcw = NULL;
SQLHSTMT hstmtw = NULL;


ware_check::ware_check(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ware_check)
{
    ui->setupUi(this);
}

ware_check::~ware_check()
{
    delete ui;
}




string qstr4str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}

void free5()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmtw);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbcw);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henvw);//释放环境
}

void connect_db5()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henvw);//申请环境
    SQLSetEnvAttr(henvw, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henvw, &hdbcw);//申请数据库连接
    retw = SQLConnect(hdbcw, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((retw == SQL_SUCCESS) || (retw == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}

void ware_check::on_pushButton_clicked()
{
    connect_db5();
    ui->textBrowser->clear();
    QString M_id = ui->lineEdit->text();
    string mid = qstr4str(M_id);
    retw = SQLAllocHandle(SQL_HANDLE_STMT, hdbcw, &hstmtw);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Medi where Mid='";
    string str3 = str2 + mid + "'";
    retw = SQLExecDirect(hstmtw, (SQLCHAR*)str1.c_str(), SQL_NTS);
    retw = SQLExecDirect(hstmtw, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (retw == SQL_SUCCESS || retw == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        while (SQLFetch(hstmtw) != SQL_NO_DATA)
        {
            SQLGetData(hstmtw, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmtw, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmtw, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmtw, 4, SQL_C_CHAR, str4, 30, &len_str4);
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
        }
        }
    free5();
}

void ware_check::on_pushButton_2_clicked()
{
    connect_db5();
    ui->textBrowser->clear();
    QString M_id = ui->lineEdit->text();
    string mid = qstr4str(M_id);
    retw = SQLAllocHandle(SQL_HANDLE_STMT, hdbcw, &hstmtw);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Medi";
    retw = SQLExecDirect(hstmtw, (SQLCHAR*)str1.c_str(), SQL_NTS);
    retw = SQLExecDirect(hstmtw, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (retw == SQL_SUCCESS || retw == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        while (SQLFetch(hstmtw) != SQL_NO_DATA)
        {
            SQLGetData(hstmtw, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmtw, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmtw, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmtw, 4, SQL_C_CHAR, str4, 30, &len_str4);
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
        }
        }
    free5();
}
