#include "init_id.h"
#include "ui_init_id.h"
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

SQLRETURN reti = NULL;
SQLHENV henvi = NULL;
SQLHDBC hdbci = NULL;
SQLHSTMT hstmti = NULL;

string qstr14str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return string(cdata);
}

void free14()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmti);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbci);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henvi);//释放环境
}

void connect_db14()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henvi);//申请环境
    SQLSetEnvAttr(henvi, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henvi, &hdbci);//申请数据库连接
    reti = SQLConnect(hdbci, (SQLTCHAR*)("zhy"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((reti == SQL_SUCCESS) || (reti == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}

init_id::init_id(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::init_id)
{
    ui->setupUi(this);
}

init_id::~init_id()
{
    delete ui;
}

void init_id::on_pushButton_clicked()
{
    connect_db14();
    QString Wid = ui->lineEdit->text();
    string worker_id = qstr14str(Wid);

    QString Wlog = ui->lineEdit_2->text();
    string wlog = qstr14str(Wlog);

    QString Wkey1 = ui->lineEdit_3->text();
    string wkey1 = qstr14str(Wkey1);

    QString Wkey2 = ui->lineEdit_4->text();
    string wkey2 = qstr14str(Wkey2);

    char log_id[20], log_key[20];

    int i;
    for( i=0;i<wlog.length();i++)
        log_id[i] = wlog[i];
    log_id[i] = '\0';
    for( i=0;i<wkey1.length();i++)
        log_key[i] = wkey1[i];
    log_key[i] = '\0';

    reti = SQLAllocHandle(SQL_HANDLE_STMT, hdbci, &hstmti);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT Wid, Wlogin FROM Workers where Wid = '";
    string str3 = str2 + worker_id + "'";
    reti = SQLExecDirect(hstmti, (SQLCHAR*)str1.c_str(), SQL_NTS);
    reti = SQLExecDirect(hstmti, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (reti == SQL_SUCCESS || reti == SQL_SUCCESS_WITH_INFO) {
        SQLCHAR str_1[10], str_2[10];
        SQLLEN len_str1, len_str2;
        while (SQLFetch(hstmti) != SQL_NO_DATA)
        {
            SQLGetData(hstmti, 1, SQL_C_CHAR, str_1, 10, &len_str1);
            SQLGetData(hstmti, 2, SQL_C_CHAR, str_2, 10, &len_str2);
        }

        string str_worker((const char*)str_1);
        str_worker.erase(str_worker.find_last_not_of(" ") + 1);
        string str_login((const char*)str_2);
        str_login.erase(str_login.find_last_not_of(" ") + 1);

        if (worker_id == str_worker){
            if(str_login == "init") {
                cout << "可以注册" << endl;
                cout << "请输入用户名:";
                int i = 0 ,j=0;

                for(i=0;i<20;i++){
                    if(log_id[i]=='\0'){
                        break;
                    }
                }
                int len1 = i;
                for(i=0;i<20;i++){
                    if(log_key[i]=='\0'){
                        break;
                    }
                }
                int len2 = i;
                free14();

                if (len1 < 6) {
                    cout << "账户长度过短"<<endl;
                    QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("id is too short, plz input 6-10 letters"));
                }

                if (len1 >10) {
                    cout << "账户长度过长"<<endl;
                    QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("id is too long, plz input 6-10 letters"));
                }

                if (len2 < 6) {
                    cout << "密码长度过短" << endl;
                    QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("key is too short, plz input 6-10 letters"));
                }

                if (len2 > 10) {
                    cout << "密码长度过长" << endl;
                    QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("key is too long, plz input 6-10 letters"));
                }

                int count1 = 0, count2 = 0;   //计数 数字和英文
                for (int k = 0; k < 10;k++) {
                      if (log_key[k] <= '9' && log_key[k] >= '0') {
                          count1++;
                      }
                      if ((log_key[k] <= 'z' && log_key[k] >= 'a' ) || (log_key[k] <= 'Z' && log_key[k] >= 'A')) {
                                   count2++;
                               }
                           }

                           if (count1 == 0) {
                               cout << "密码中必须同时包含数字和字母，当前不包含数字"<<endl;
                               QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("password must include letters and numbers,plz input again"));
                           }
                           if (count2 == 0) {
                               cout << "密码中必须同时包含数字和字母，当前不包含字母"<<endl;
                               QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("password must include letters and numbers,plz input again"));
                           }

                           cout << count1 << endl << count2;
                           if (count1 != 0 && count2 != 0) {
                               string str_key = log_key;
                               string str_id = log_id;
                               //printf("1:%s,2:%s\n",str_key,wkey2);
                               cout << str_key<<endl;
                               cout << wkey2<<endl;
                               if (str_key == wkey2) {
                                   reti = SQLAllocHandle(SQL_HANDLE_STMT, hdbci, &hstmti);//申请句柄
                                   string str1 = "use Medicine";
                                   string str2 = "UPDATE Workers SET Wlogin = '" + str_id + "' ,Wkey = '" + str_key + "' Where Wid = '" + worker_id + "'";

                                   reti = SQLExecDirect(hstmti, (SQLCHAR*)str1.c_str(), SQL_NTS);
                                   reti = SQLExecDirect(hstmti, (SQLCHAR*)str2.c_str(), SQL_NTS);
                                   free14();
                                   QMessageBox::information(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("success"));
                                   cout << "注册成功！" << endl;
                               }
                               else {
                                   cout << "两次密码不一致，请重新输入";
                                   QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("two passwords are different"));
                               }
                           }
              }
              else {
                cout << "账户已存在，请登录" << endl;
                QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("id already exist"));
              }
        }
        else{
            QMessageBox::critical(this, QString::fromLocal8Bit("warning"), QString::fromLocal8Bit("not a employee"));
        }
    }
}
