#include "hr.h"
#include "ui_hr.h"
#include "hr_check.h"
#include "hr_manage.h"
#include "hcheck_work.h"
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

SQLRETURN retf = NULL;
SQLHENV henvf = NULL;
SQLHDBC hdbcf = NULL;
SQLHSTMT hstmtf = NULL;

void free9()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmtf);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbcf);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henvf);//释放环境
}

void connect_db9()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henvf);//申请环境
    SQLSetEnvAttr(henvf, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henvf, &hdbcf);//申请数据库连接
    retf = SQLConnect(hdbcf, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((retf == SQL_SUCCESS) || (retf == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}

hr::hr(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hr)
{
    ui->setupUi(this);
}

hr::~hr()
{
    delete ui;
}

void hr::on_pushButton_clicked()
{
    hr_check *hc = new hr_check;
    hc->show();
}

void hr::on_pushButton_2_clicked()
{
    connect_db9();
    ui->textBrowser->clear();
    retf = SQLAllocHandle(SQL_HANDLE_STMT, hdbcf, &hstmtf);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT SUM(profit) AS total from Profit";
    retf = SQLExecDirect(hstmtf, (SQLCHAR*)str1.c_str(), SQL_NTS);
    retf = SQLExecDirect(hstmtf, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (retf == SQL_SUCCESS || retf == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10];
        SQLLEN len_str1;
        cout << "总盈利" << endl;
        while (SQLFetch(hstmtf) != SQL_NO_DATA) {
            SQLGetData(hstmtf, 1, SQL_C_CHAR, str1, 10, &len_str1);
        }
        string str_1((const char*)str1);
        ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_1.data()));
    }
    free9();
}

void hr::on_pushButton_4_clicked()
{
    hr_manage *hm = new hr_manage;
    hm->show();
}

void hr::on_pushButton_3_clicked()
{
    hcheck_work *hcw = new hcheck_work;
    hcw->show();
}
