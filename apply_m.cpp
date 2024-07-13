#include "apply_m.h"
#include "ui_apply_m.h"
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
#include <cstring>
using namespace std;
using std::to_string;

SQLRETURN reta = NULL;
SQLHENV henva = NULL;
SQLHDBC hdbca = NULL;
SQLHSTMT hstmta = NULL;

int h=0;
int Ap_id[20] = {0};
int Ap_num[20];

apply_m::apply_m(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::apply_m)
{
    ui->setupUi(this);
}

apply_m::~apply_m()
{
    delete ui;
}

void connect_db3()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henva);//申请环境
    SQLSetEnvAttr(henva, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henva, &hdbca);//申请数据库连接
    reta = SQLConnect(hdbca, (SQLTCHAR*)(""), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((reta == SQL_SUCCESS) || (reta == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}

void free3()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmta);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbca);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henva);//释放环境
}

string qstr3str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}

void apply_mid_new(string Aid, string Mid, string Mcount) {
    string state = "unfinished";
    reta = SQLAllocHandle(SQL_HANDLE_STMT, hdbca, &hstmta);//申请句柄
    string str1 = "use Medicine";//第一句要执行的SQL语句
    string str2 = "insert into Medi_apply values ('";//第二句要执行的SQL语句
    string str3 = str2 + Aid + "','" + Mid + "','" + Mcount +"','"+state+"')";
    reta = SQLExecDirect(hstmta, (SQLCHAR*)str1.c_str(), SQL_NTS);
    reta = SQLExecDirect(hstmta, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (reta == SQL_SUCCESS || reta == SQL_SUCCESS_WITH_INFO) {
        cout << "申请成功！" << endl;
    }
    else {
        cout << "申请失败！" << endl;
    }
    free3();
}

int check_list_num(){
    connect_db3();
    int id_num;
    reta = SQLAllocHandle(SQL_HANDLE_STMT, hdbca, &hstmta);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Medi_apply";
    reta = SQLExecDirect(hstmta, (SQLCHAR*)str1.c_str(), SQL_NTS);
    reta = SQLExecDirect(hstmta, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (reta == SQL_SUCCESS || reta == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[20];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        while (SQLFetch(hstmta) != SQL_NO_DATA)
        {
            SQLGetData(hstmta, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmta, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmta, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmta, 3, SQL_C_CHAR, str4, 20, &len_str4);
            string str_1((const char*)str1);
            string str_2((const char*)str2);
            string str_3((const char*)str3);
            string str_4((const char*)str4);
            istringstream sss(str_1);
            sss >> id_num;
            }
        }
        free3();
        return id_num+1;
}

void apply_m::on_pushButton_clicked()
{
    connect_db3();
    QString mid = ui->lineEdit->text();
    string m_id = qstr3str(mid);
    QString mnum = ui->lineEdit_2->text();
    string m_num = qstr3str(mnum);
    ui->textBrowser->insertPlainText(QString::fromLocal8Bit(m_id.data())+='\t');
    ui->textBrowser->insertPlainText(QString::fromLocal8Bit(m_num.data())+='\n');
    istringstream ss(m_id);
    ss >> Ap_id[h];
    istringstream sss(m_num);
    sss >> Ap_num[h];
    h++;
}

void apply_m::on_pushButton_2_clicked()
{
    h=0;
    for(int j=0;Ap_id[j]!=0;j++){
        int Aid = check_list_num();
        printf("aid:%d\n",Aid);
        apply_mid_new(to_string(Aid),to_string(Ap_id[j]),to_string(Ap_num[j]));
    }
}
