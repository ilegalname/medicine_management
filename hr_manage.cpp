#include "hr_manage.h"
#include "ui_hr_manage.h"
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

SQLRETURN rethm = NULL;
SQLHENV henvhm = NULL;
SQLHDBC hdbchm = NULL;
SQLHSTMT hstmthm = NULL;

int check_id(string if_id);

string qstr10str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}

void free10()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmthm);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbchm);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henvhm);//释放环境
}

void connect_db10()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henvhm);//申请环境
    SQLSetEnvAttr(henvhm, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henvhm, &hdbchm);//申请数据库连接
    rethm = SQLConnect(hdbchm, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((rethm == SQL_SUCCESS) || (rethm == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}

hr_manage::hr_manage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::hr_manage)
{
    ui->setupUi(this);
}

hr_manage::~hr_manage()
{
    delete ui;
}

void hr_manage::on_pushButton_clicked()
{
        QString M_id = ui->lineEdit->text();
        string Bno = qstr10str(M_id);
        int if_correct = check_id(Bno);
        if(if_correct == 1){
            QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("id exist, plz rename"));
        }
        else{
            connect_db10();
            rethm = SQLAllocHandle(SQL_HANDLE_STMT, hdbchm, &hstmthm);//申请句柄
            string str1 = "use Medicine";//第一句要执行的SQL语句
            string str2 = "insert into Workers (Wid, Wname, Wage, Wplace, Wsalary, Wlogin, Wkey) values ('";
            QString M_n = ui->lineEdit_2->text();
            string Bname = qstr10str(M_n);
            QString M_a = ui->lineEdit_3->text();
            string Bage = qstr10str(M_a);
            string Bplace;
            int bs = 3000;
            string Bsalary = to_string(bs);
            string Blog = "init";
            string Bkey = "666";
            if(ui->comboBox->currentIndex()==0){
                Bplace = "store";
            }
            else if(ui->comboBox->currentIndex()==1){
                Bplace = "warehouse";
            }
            else{
                Bplace = "HR";
            }
            string str3 = str2 + Bno + "','" + Bname + "','" + Bage + "','" + Bplace + "','" + Bsalary + "','" + Blog + "','" + Bkey + "')";
            rethm = SQLExecDirect(hstmthm, (SQLCHAR*)str1.c_str(), SQL_NTS);
            rethm = SQLExecDirect(hstmthm, (SQLCHAR*)str3.c_str(), SQL_NTS);
            if (rethm == SQL_SUCCESS || rethm == SQL_SUCCESS_WITH_INFO) {
                cout << "员工添加成功！" << endl;
            }
            else {
                cout << "员工添加失败！" << endl;
            }
        }
        free10();
}

int check_id(string if_id){
    connect_db10();
    rethm = SQLAllocHandle(SQL_HANDLE_STMT, hdbchm, &hstmthm);//申请句柄
        string str1 = "use Medicine";
        string str2 = "SELECT Wid FROM Workers where Wid = '";
        string str3 = str2 + if_id +"'";
        rethm = SQLExecDirect(hstmthm, (SQLCHAR*)str1.c_str(), SQL_NTS);
        rethm = SQLExecDirect(hstmthm, (SQLCHAR*)str3.c_str(), SQL_NTS);
        if (rethm == SQL_SUCCESS || rethm == SQL_SUCCESS_WITH_INFO)
        {
            SQLCHAR str1[10];
            SQLLEN len_str1;
            while (SQLFetch(hstmthm) != SQL_NO_DATA)
            {
                SQLGetData(hstmthm, 1, SQL_C_CHAR, str1, 10, &len_str1);
            }
            string str_login((const char*)str1);
            str_login.erase(str_login.find_last_not_of(" ") + 1);
            if(str_login == if_id){
                free10();
                return 1;
            }
            else{
                free10();
                return 0;
            }
        }
        else
            free10();
            return 0;
}

void hr_manage::on_pushButton_2_clicked()
{
    connect_db10();
    rethm = SQLAllocHandle(SQL_HANDLE_STMT, hdbchm, &hstmthm);//申请句柄
    string str1 = "use Medicine";//第一句要执行的SQL语句
    string str2 = "delete from Workers Where Wid = '";//第二句要执行的SQL语句
    QString W_id = ui->lineEdit_4->text();
    string Wno = qstr10str(W_id);
    string str3 = str2 + Wno +"'";
        rethm = SQLExecDirect(hstmthm, (SQLCHAR*)str1.c_str(), SQL_NTS);
        rethm = SQLExecDirect(hstmthm, (SQLCHAR*)str3.c_str(), SQL_NTS);
        if (rethm == SQL_SUCCESS || rethm == SQL_SUCCESS_WITH_INFO) {
            cout << "解雇成功！" << endl;
        }
        else {
            cout << "解雇失败！" << endl;
            QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("no such worker"));
        }
    //}
        free10();
}

void hr_manage::on_pushButton_3_clicked()
{
    connect_db10();
    rethm = SQLAllocHandle(SQL_HANDLE_STMT, hdbchm, &hstmthm);//申请句柄
    QString W_id = ui->lineEdit_5->text();
    string Wno = qstr10str(W_id);
    QString W_sa = ui->lineEdit_6->text();
    string Wsa = qstr10str(W_sa);
    string str1 = "use Medicine";
    string str2 = "UPDATE Workers SET Wsalary = " + Wsa + " Where Workers.Wid = " + Wno;

    rethm = SQLExecDirect(hstmthm, (SQLCHAR*)str1.c_str(), SQL_NTS);
    rethm = SQLExecDirect(hstmthm, (SQLCHAR*)str2.c_str(), SQL_NTS);
    cout << "更新成功！" << endl;
    if (rethm == SQL_SUCCESS || rethm == SQL_SUCCESS_WITH_INFO) {
        cout << "更改工资成功！" << endl;
    }
    else {
        cout << "更改工资失败！" << endl;
        QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("no such worker"));
    }
    free10();
}

void hr_manage::on_pushButton_4_clicked()
{
    connect_db10();
    rethm = SQLAllocHandle(SQL_HANDLE_STMT, hdbchm, &hstmthm);//申请句柄
    QString W_id = ui->lineEdit_7->text();
    string Wno = qstr10str(W_id);
    string Wpa;
    if(ui->comboBox_2->currentIndex()==0){
        Wpa = "store";
    }
    else if(ui->comboBox_2->currentIndex()==1){
        Wpa = "warehouse";
    }
    else{
        Wpa = "HR";
    }
    string str1 = "use Medicine";
    string str2 = "UPDATE Workers SET Wplace = '" + Wpa + "' Where Workers.Wid = " + Wno;
    cout<<Wno<<endl;
    rethm = SQLExecDirect(hstmthm, (SQLCHAR*)str1.c_str(), SQL_NTS);
    rethm = SQLExecDirect(hstmthm, (SQLCHAR*)str2.c_str(), SQL_NTS);
    cout << "更新成功！" << endl;
    if (rethm == SQL_SUCCESS || rethm == SQL_SUCCESS_WITH_INFO) {
        cout << "解雇成功！" << endl;
    }
    else {
        cout << "解雇失败！" << endl;
        QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("no such worker"));
    }
    free10();
}
