#include "hr_check.h"
#include "ui_hr_check.h"
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

SQLRETURN rethc = NULL;
SQLHENV henvhc = NULL;
SQLHDBC hdbchc = NULL;
SQLHSTMT hstmthc = NULL;

string qstr7str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}

void free7()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmthc);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbchc);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henvhc);//释放环境
}

void connect_db7()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henvhc);//申请环境
    SQLSetEnvAttr(henvhc, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henvhc, &hdbchc);//申请数据库连接
    rethc = SQLConnect(hdbchc, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((rethc == SQL_SUCCESS) || (rethc == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}

hr_check::hr_check(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hr_check)
{
    ui->setupUi(this);
}

hr_check::~hr_check()
{
    delete ui;
}

void hr_check::on_pushButton_2_clicked()
{
    connect_db7();
    ui->textBrowser->clear();
    rethc = SQLAllocHandle(SQL_HANDLE_STMT, hdbchc, &hstmthc);//申请句柄
        string str1 = "use Medicine";
        string str2 = "SELECT Wid, Wname, Wage, Wplace, Wsalary FROM Workers";
        rethc = SQLExecDirect(hstmthc, (SQLCHAR*)str1.c_str(), SQL_NTS);
        rethc = SQLExecDirect(hstmthc, (SQLCHAR*)str2.c_str(), SQL_NTS);
        if (rethc == SQL_SUCCESS || rethc == SQL_SUCCESS_WITH_INFO)
        {
            SQLCHAR str1[10], str2[12], str3[20], str4[30], str5[30];
            SQLLEN len_str1, len_str2, len_str3, len_str4, len_str5;
            while (SQLFetch(hstmthc) != SQL_NO_DATA)
            {
                SQLGetData(hstmthc, 1, SQL_C_CHAR, str1, 10, &len_str1);
                SQLGetData(hstmthc, 2, SQL_C_CHAR, str2, 12, &len_str2);
                SQLGetData(hstmthc, 3, SQL_C_CHAR, str3, 20, &len_str3);
                SQLGetData(hstmthc, 4, SQL_C_CHAR, str4, 30, &len_str4);
                SQLGetData(hstmthc, 5, SQL_C_CHAR, str5, 30, &len_str5);
                string str = "  ";
                string str_1((const char*)str1);
                string str_2((const char*)str2);
                string str_3((const char*)str3);
                string str_4((const char*)str4);
                string str_5((const char*)str5);
                ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str.data())+=' ');
                ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_1.data())+='\t');
                ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_2.data())+='\t');
                ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_3.data())+='\t');
                ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_4.data())+='\t');
                ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_5.data())+='\n');
                printf("%s\t%s\t%s\t%s\n", str1, str2, str3, str4);
            }
        }
        free7();
}
