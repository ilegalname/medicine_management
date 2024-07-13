#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ph.h"
#include "ware.h"
#include "hr.h"
#include "init_id.h"
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
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QMessageBox>

using namespace std;
using std::to_string;

#ifdef _WIN64
typedef INT64 SQLLEN;
typedef UINT64 SQLULEN;
#else
#define SQLLEN SQLINTEGER
#define SQLULEN SQLUINTEGER
#endif

SQLRETURN ret = NULL;
SQLHENV henv = NULL;
SQLHDBC hdbc = NULL;
SQLHSTMT hstmt = NULL;

int current_id;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void connect();


/***********************数据库连接与释放**************************/
//free
void free()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
}

//连接数据库
void connect_db()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);//申请环境
    SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);//申请数据库连接
    ret = SQLConnect(hdbc, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((ret == SQL_SUCCESS) || (ret == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}

string qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}


void MainWindow::on_pushButton_clicked()              //主界面登录函数
{
    connect_db();
    QString startstation = ui->lineEdit->text();
    string log_id = qstr2str(startstation);

    QString endstation = ui->lineEdit_2->text();
    string log_key = qstr2str(endstation);

    if(log_id == "init"){
        if(log_key == "666"){
            QMessageBox::warning(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("success"));
            init_id *ii = new init_id;
            ii->show();
        }
        else{
            QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("wrong password"));
        }
    }
    else{
    //QString log_place = "store";
    //string wp = qstr2str(log_place);

    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string worker_id;
    string str1 = "use Medicine";
    string str2 = "SELECT Wlogin,Wkey,Wname,Wplace,Wid FROM Workers where Wlogin = '";
    string str3 = str2 + log_id + "'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        SQLCHAR str_1[10], str_2[10], str_3[10], str_4[10];
        SQLLEN len_str1, len_str2,len_str3,len_str4;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str_1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str_2, 10, &len_str2);
            SQLGetData(hstmt, 3, SQL_C_CHAR, str_3, 10, &len_str3);
            SQLGetData(hstmt, 4, SQL_C_CHAR, str_4, 10, &len_str4);
        }
        string str_login((const char*)str_1);
        string str_key((const char*)str_2);
        string str_id((const char*)str_3);
        string str_place((const char*)str_4);

        str_id.erase(str_id.find_last_not_of(" ") + 1);
        istringstream sss(str_id);
        sss >> current_id;
        printf("id:%d",current_id);
        cout<<str_place<<endl;
        str_key.erase(str_key.find_last_not_of(" ") + 1);
        str_login.erase(str_login.find_last_not_of(" ") + 1);
        str_place.erase(str_place.find_last_not_of(" ") + 1);
        cout<<str_place<<endl;
        if (log_id == str_login) {
            if (log_key == str_key) {
                if(str_place == "store"){
                    ph *ph_w = new ph;
                    QMessageBox::information(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("success"));
                    ph_w->show();

                }
                else if(str_place == "warehous"){
                    ware *wh = new ware;
                    QMessageBox::information(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("success"));
                    wh->show();
                }
                else{
                    hr *hr_w = new hr;
                    QMessageBox::information(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("success"));
                    hr_w->show();
                }
            }
            else {
                cout << "密码错误，请重新输入" << endl;
                QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("wrong password"));
            }
        }
        else {
            QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("id not exist, plz init"));
        }
    }
    free();
    }
}
