#include "ware_add.h"
#include "ui_ware_add.h"
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
#include <QCoreApplication>
#include <QVBoxLayout>
#include <QMessageBox>

using namespace std;
using std::to_string;

SQLRETURN retwa = NULL;
SQLHENV henvwa = NULL;
SQLHDBC hdbcwa = NULL;
SQLHSTMT hstmtwa = NULL;

int k=0;
int money = 0;
int in_mid[20]={0};
int in_mnum[20];
int in_mon[20];
string in_mediname[20];

void add_medicine(string Bno, string Bname, string Bsum, string Bprice, string Wid, string Bdate);

ware_add::ware_add(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ware_add)
{
    ui->setupUi(this);
}

ware_add::~ware_add()
{
    delete ui;
}

string qstr6str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}

void free6()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmtwa);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbcwa);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henvwa);//释放环境
}

//连接数据库
void connect_db6()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henvwa);//申请环境
    SQLSetEnvAttr(henvwa, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henvwa, &hdbcwa);//申请数据库连接
    retwa = SQLConnect(hdbcwa, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((retwa == SQL_SUCCESS) || (retwa == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}


void ware_add::on_pushButton_clicked()
{
    int single_price = 0;
    int current_num = 0;
    connect_db6();
    QString M_id = ui->lineEdit->text();
    string mid = qstr6str(M_id);
    QString M_num = ui->lineEdit_2->text();
    string mnum = qstr6str(M_num);
    istringstream cc(mid);
    int bi;
    cc >> bi;
    int want_num;
    istringstream s(mnum);
    s >> want_num;
    retwa = SQLAllocHandle(SQL_HANDLE_STMT, hdbcwa, &hstmtwa);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT * FROM Medi where Mid='";
    string str3 = str2 + mid + "'";
    retwa = SQLExecDirect(hstmtwa, (SQLCHAR*)str1.c_str(), SQL_NTS);
    retwa = SQLExecDirect(hstmtwa, (SQLCHAR*)str3.c_str(), SQL_NTS);

    if(bi==1||bi==2||bi==3||bi==5||bi==6||bi==7){
        if (retwa == SQL_SUCCESS || retwa == SQL_SUCCESS_WITH_INFO)
        {
            SQLCHAR str1[10], str2[12], str3[20], str4[20];
            SQLLEN len_str1, len_str2, len_str3, len_str4;
            while (SQLFetch(hstmtwa) != SQL_NO_DATA)
            {
                SQLGetData(hstmtwa, 1, SQL_C_CHAR, str1, 10, &len_str1);
                SQLGetData(hstmtwa, 2, SQL_C_CHAR, str2, 12, &len_str2);
                SQLGetData(hstmtwa, 3, SQL_C_CHAR, str3, 20, &len_str3);
                SQLGetData(hstmtwa, 4, SQL_C_CHAR, str4, 20, &len_str4);
            }
            string str_1((const char*)str1);
            string str_2((const char*)str2);
            string str_3((const char*)str3);
            string str_4((const char*)str4);

            istringstream ss(str_4);
            ss >> single_price;
            istringstream sc(str_3);
            sc >> current_num;

            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_1.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_2.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(mnum.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_4.data())+='\n');

            in_mid[k] = bi;
            in_mnum[k] = want_num + current_num;
            in_mediname[k]=str_2;
            in_mon[k]=single_price;
            k++;
        }
    }
    else{
        QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("no such medicine"));
    }
}

void ware_add::on_pushButton_2_clicked()
{
    k=0;
    int h = 0;
    QString M_id = ui->lineEdit_3->text();
    string d = qstr6str(M_id);
    for(h=0;in_mid[h]!=0;h++){
        add_medicine(to_string(in_mid[h]),in_mediname[h],to_string(in_mnum[h]),to_string(in_mon[h]),to_string(current_id),d);
    }
    ui->textBrowser->clear();
}


void add_medicine(string Bno, string Bname, string Bsum, string Bprice, string Wid, string Bdate){
    //更新库存
       connect_db6();
       retwa = SQLAllocHandle(SQL_HANDLE_STMT, hdbcwa, &hstmtwa);//申请句柄
       string str1 = "use Medicine";//第一句要执行的SQL语句
       string str3 = "UPDATE Medi SET Mcount = " + Bsum + " Where Medi.Mid = " + Bno;
       retwa = SQLExecDirect(hstmtwa, (SQLCHAR*)str1.c_str(), SQL_NTS);
       retwa = SQLExecDirect(hstmtwa, (SQLCHAR*)str3.c_str(), SQL_NTS);
       if (retwa == SQL_SUCCESS || retwa == SQL_SUCCESS_WITH_INFO) {
           cout << "药品上架成功！" << endl;
       }
       else {
           cout << "药品上架失败！" << endl;
       }
       free6();

       //更新记录
       retwa = SQLAllocHandle(SQL_HANDLE_STMT, hdbcwa, &hstmtwa);//申请句柄
       string str4 = "use Medicine";//第一句要执行的SQL语句
       string str5 = "insert into Medi_inlist values ('";//第二句要执行的SQL语句
       string str6 = str5 + Bno + "','" + Bname + "','" + Bdate + "','" + Wid + "','" + Bsum + "')";
       retwa = SQLExecDirect(hstmtwa, (SQLCHAR*)str4.c_str(), SQL_NTS);
       retwa = SQLExecDirect(hstmtwa, (SQLCHAR*)str6.c_str(), SQL_NTS);
       if (retwa == SQL_SUCCESS || retwa == SQL_SUCCESS_WITH_INFO) {
           cout << "员工进货记录更新成功！" << endl;
       }
       else {
           cout << "员工进货记录更新失败！" << endl;
       }
       free6();
}

