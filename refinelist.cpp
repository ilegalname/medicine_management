#include "refinelist.h"
#include "ui_refinelist.h"
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
using namespace std;
using std::to_string;

SQLRETURN retr = NULL;
SQLHENV henvr = NULL;
SQLHDBC hdbcr = NULL;
SQLHSTMT hstmtr = NULL;

string qstr13str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}

void free13()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmtr);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbcr);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henvr);//释放环境
}

void connect_db13()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henvr);//申请环境
    SQLSetEnvAttr(henvr, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henvr, &hdbcr);//申请数据库连接
    retr = SQLConnect(hdbcr, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((retr == SQL_SUCCESS) || (retr == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}

refinelist::refinelist(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::refinelist)
{
    ui->setupUi(this);
}

refinelist::~refinelist()
{
    delete ui;
}

void refinelist::on_pushButton_clicked()
{
    QString aid = ui->lineEdit->text();
    string a_id = qstr13str(aid);
    QString mnum = ui->lineEdit_2->text();
    string m_num = qstr13str(mnum);
    connect_db13();
    retr = SQLAllocHandle(SQL_HANDLE_STMT, hdbcr, &hstmtr);//申请句柄
    string str1 = "use Medicine";
    string str2 = "UPDATE Medi_apply SET Mstate = 'unfinished' where Medi_apply.Aid =" + a_id;
    string str3 = "UPDATE Medi_apply SET Mnum =" + m_num + "where Medi_apply.Aid =" + a_id;

    retr = SQLExecDirect(hstmtr, (SQLCHAR*)str1.c_str(), SQL_NTS);
    retr = SQLExecDirect(hstmtr, (SQLCHAR*)str2.c_str(), SQL_NTS);
    retr = SQLExecDirect(hstmtr, (SQLCHAR*)str3.c_str(), SQL_NTS);
    free13();
}
