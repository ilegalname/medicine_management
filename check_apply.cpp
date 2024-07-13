#include "check_apply.h"
#include "ui_check_apply.h"
#include "refinelist.h"
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

SQLRETURN retca = NULL;
SQLHENV henvca = NULL;
SQLHDBC hdbcca = NULL;
SQLHSTMT hstmtca = NULL;


using namespace std;
using std::to_string;

check_apply::check_apply(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::check_apply)
{
    ui->setupUi(this);
}

check_apply::~check_apply()
{
    delete ui;
}

void free4()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmtca);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbcca);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henvca);//释放环境
}

//连接数据库
void connect_db4()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henvca);//申请环境
    SQLSetEnvAttr(henvca, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henvca, &hdbcca);//申请数据库连接
    retca = SQLConnect(hdbcca, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((retca == SQL_SUCCESS) || (retca == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}


void check_apply::on_pushButton_clicked()
{
    ui->textBrowser->clear();
    connect_db4();
    retca = SQLAllocHandle(SQL_HANDLE_STMT, hdbcca, &hstmtca);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Medi_apply";
    retca = SQLExecDirect(hstmtca, (SQLCHAR*)str1.c_str(), SQL_NTS);
    retca = SQLExecDirect(hstmtca, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (retca == SQL_SUCCESS || retca == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[20];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        while (SQLFetch(hstmtca) != SQL_NO_DATA)
        {
            SQLGetData(hstmtca, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmtca, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmtca, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmtca, 4, SQL_C_CHAR, str4, 20, &len_str4);
            string str_1((const char*)str1);
            string str_2((const char*)str2);
            string str_3((const char*)str3);
            string str_4((const char*)str4);
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_1.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_2.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_3.data())+='\t');
            ui->textBrowser->insertPlainText(QString::fromLocal8Bit(str_4.data())+='\n');
            }
        }
        free4();
}

void check_apply::on_pushButton_2_clicked()
{
    //删除申请
    retca = SQLAllocHandle(SQL_HANDLE_STMT, hdbcca, &hstmtca);//申请句柄
    string str3 = "use Medicine";//第一句要执行的SQL语句
    string str4 = "delete from Medi_apply Where Mstate = 'finnished'";//第二句要执行的SQL语句

    retca = SQLExecDirect(hstmtca, (SQLCHAR*)str3.c_str(), SQL_NTS);
    retca = SQLExecDirect(hstmtca, (SQLCHAR*)str4.c_str(), SQL_NTS);
    if (retca == SQL_SUCCESS || retca == SQL_SUCCESS_WITH_INFO) {
        cout << "删除成功！" << endl;
    }
    else {
        cout << "删除失败！" << endl;
    }
    free4();
}


void check_apply::on_pushButton_4_clicked()
{
    string id_apply;
    string state_apply;
    string num_apply;
    string apply_id;
    //查看申请完成状态
    retca = SQLAllocHandle(SQL_HANDLE_STMT, hdbcca, &hstmtca);//申请句柄
    string str1 = "use Medicine";//第一句要执行的SQL语句
    string str2 = "SELECT* FROM Medi_apply Where Mstate = 'unchecked'";//第二句要执行的SQL语句

    retca = SQLExecDirect(hstmtca, (SQLCHAR*)str1.c_str(), SQL_NTS);
    retca = SQLExecDirect(hstmtca, (SQLCHAR*)str2.c_str(), SQL_NTS);

    if (retca == SQL_SUCCESS || retca == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str_1[10], str_2[20], str_3[10], str_4[10];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        cout << "申请药品编号" << "    " << "申请药品名称" << "    " << "申请药品数量" << "    " << "申请状态" << endl;
        while (SQLFetch(hstmtca) != SQL_NO_DATA)
        {
            SQLGetData(hstmtca, 1, SQL_C_CHAR, str_1, 10, &len_str1);
            SQLGetData(hstmtca, 2, SQL_C_CHAR, str_2, 20, &len_str2);
            SQLGetData(hstmtca, 3, SQL_C_CHAR, str_3, 10, &len_str3);
            SQLGetData(hstmtca, 4, SQL_C_CHAR, str_4, 10, &len_str4);
        }
        string aid((const char*)str_1);
        aid.erase(aid.find_last_not_of(" ") + 1);
        apply_id = aid;

        string str_id((const char*)str_2);
        str_id.erase(str_id.find_last_not_of(" ") + 1);
        id_apply = str_id;

        string str_num((const char*)str_3);
        str_num.erase(str_num.find_last_not_of(" ") + 1);
        num_apply = str_num;
        int apply_num = 0;
        istringstream ss(num_apply);
        ss >> apply_num;
    }
    free4();

    //修改药房库存
    retca = SQLAllocHandle(SQL_HANDLE_STMT, hdbcca, &hstmtca);//申请句柄
    //更新药房药品数量
    string str3 = "use Medicine";
    string str4 = "UPDATE Mediout SET Mnum =( Mnum + " + num_apply + ") Where Mediout.Mid = " + id_apply;
    retca = SQLExecDirect(hstmtca, (SQLCHAR*)str3.c_str(), SQL_NTS);
    retca = SQLExecDirect(hstmtca, (SQLCHAR*)str4.c_str(), SQL_NTS);
    free4();

    retca = SQLAllocHandle(SQL_HANDLE_STMT, hdbcca, &hstmtca);//申请句柄
    //更新药房药品数量
    string str5 = "use Medicine";
    string str6 = "UPDATE Medi_apply SET Mstate = 'finnished' where Mstate = 'unchecked'";
    retca = SQLExecDirect(hstmtca, (SQLCHAR*)str5.c_str(), SQL_NTS);
    retca = SQLExecDirect(hstmtca, (SQLCHAR*)str6.c_str(), SQL_NTS);
    free4();
}

void check_apply::on_pushButton_3_clicked()
{
    refinelist *rl = new refinelist;
    rl->show();
}
