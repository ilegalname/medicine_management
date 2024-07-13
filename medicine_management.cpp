#undef UNICODE
#undef _UNICODE
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <iomanip>
#include <sstream>



#ifdef _WIN64
typedef INT64 SQLLEN;
typedef UINT64 SQLULEN;
#else
#define SQLLEN SQLINTEGER
#define SQLULEN SQLUINTEGER
#endif

using namespace std;

SQLRETURN ret = NULL;
SQLHENV henv = NULL;
SQLHDBC hdbc = NULL;
SQLHSTMT hstmt = NULL;


int medinum = 0;   //补充数量
int mediid = 0;   //补充id
int num = 0;   //仓库库存
int mediout_num; //药房库存
int flag_medi;  //仓库是否可以补货
int flag_mediout;  //药房是否可以补货
string medi_outno;  //卖出id
string medi_outname;   //卖出名字

void free();
void connect();

void login();
void reg();

void print_medi();
void print_medi_all();
void update_medi();
void to_mediout();
void to_mediout_new();
void tip();
void update_state();

void print_mediout();
void print_mediout_all();
void sold_mid();
void apply_mid();
void update_mediout();

void print_workers();
void print_profit();
void view_medi_sold();
void view_medi_inlist();
void insert_worker();
void delete_worker();
void update_salary();



/***********************数据库连接与释放**************************/
//free
void free()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
    SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
}

//连接数据库
void connect()//数据库连接函数
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);//申请环境
    SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
    SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);//申请数据库连接
    ret = SQLConnect(hdbc, (SQLTCHAR*)("sql"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((ret == SQL_SUCCESS) || (ret == SQL_SUCCESS_WITH_INFO))
    {
        cout << "数据库连接成功!" << endl;
    }
    else
    {
        cout << "数据库连接失败！" << endl;
    }
}



/***********************员工注册及登录账号**************************/
//登录
void login() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string log_id, log_key, worker_id;
    cout << "------智能药品库管理系统------" << endl;
    cout << "请输入用户名:";
    cin >> log_id;
    cout << "请输入密码:";
    cin >> log_key;
    string str1 = "use Medicine";
    string str2 = "SELECT Wlogin,Wkey FROM Workers where Wlogin = '";
    string str3 = str2 + log_id + "'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        SQLCHAR str_1[10], str_2[10];
        SQLLEN len_str1, len_str2;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str_1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str_2, 10, &len_str2);
        }
        string str_login((const char*)str_1);
        string str_key((const char*)str_2);
        str_key.erase(str_key.find_last_not_of(" ") + 1);
        str_login.erase(str_login.find_last_not_of(" ") + 1);
        if (log_id == str_login) {
            if (log_key == str_key) {
                cout << "登录成功！" << endl;
            }
            else {
                cout << "密码错误，请重新输入" << endl;
            }
        }
        else {
            cout << "用户名不存在，请注册" << endl;
            reg();
        }
    }
    free();
}

//注册
void reg() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string log_id, log_key, log_kkey,worker_id;
    cout << "请输入您的工号：";
    cin >> worker_id;
    string str1 = "use Medicine";
    string str2 = "SELECT Wid FROM Workers where Wid = '";
    string str3 = str2 + worker_id + "'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        SQLCHAR str_1[10];
        SQLLEN len_str1;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str_1, 10, &len_str1);
        }
        string str_worker((const char*)str_1);
        str_worker.erase(str_worker.find_last_not_of(" ") + 1);
        if (worker_id == str_worker) {
            cout << "可以注册" << endl;
            cout << "请输入用户名:";
            cin >> log_id;
            cout << "请输入密码";
            cin >> log_key;
            cout << "请确认密码";
            cin >> log_kkey;
            free();
            if (log_key == log_kkey) {
                ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
                string str1 = "use Medicine";
                //string str2 = "UPDATE Workers SET Mnum = " + to_string(num_update) + " Where Mediout.Mid = " + medi_outno;

                ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
                ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
                free();
            }
            else {
               // cout << "两次密码不一致，请重新输入";
            }
        }
    }


}



/***********************仓库工作人员相关操作**************************/
//仓库检查是否有该药品
void print_medi()
{
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT*FROM Medi where Mid='";
    cout << "请输入药品编号" << endl;
    string Sno;
    cin >> Sno;
    string str3 = str2 + Sno + "'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3,len_str4;
        cout << "药品编号" << "    " << "药品名称" << "    " << "药品数量"  <<"     " << "药品进价" << endl;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmt, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmt, 4, SQL_C_CHAR, str4, 30, &len_str4);
            printf("%s\t%s\t%s\t%s\n", str1, str2, str3, str4);
        }
    }
    free();
}

//检查仓库所有药品库存
void print_medi_all()
{
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT*FROM Medi";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        cout << "药品编号" << "    " << "药品名称" << "    " << "药品数量" << "     " << "药品进价" << endl;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmt, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmt, 4, SQL_C_CHAR, str4, 30, &len_str4);
            printf("%s\t%s\t%s\t%s\n", str1, str2, str3, str4);
        }
    }
    free();
}

//进货清单+更新仓库药品库存+更新员工进货记录
void update_medi()
{   
    //更新库存
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";//第一句要执行的SQL语句
    string str2 = "insert into Medi values ('";//第二句要执行的SQL语句
    cout << "请依次输入药品编号、药品名称、药品数量、药品进价、工号、进货时间" << endl;
    cout << "例：55 DEA 12 56 66 2024-9-6" << endl;
    string Bno, Bname, Bsum, Bprice, Wid, Bdate;
    cin >> Bno >> Bname >> Bsum >> Bprice >> Wid >> Bdate;
    string str3 = str2 + Bno + "','" + Bname + "','" + Bsum + "','" + Bprice + "')";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "药品上架成功！" << endl;
    }
    else {
        cout << "药品上架失败！" << endl;
    }
    free();

    //更新记录
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str4 = "use Medicine";//第一句要执行的SQL语句
    string str5 = "insert into Medi_inlist values ('";//第二句要执行的SQL语句
    string str6 = str5 + Bno + "','" + Bname + "','" + Bdate + "','" + Wid + "','" + Bsum + "')";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str4.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str6.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "员工进货记录更新成功！" << endl;
    }
    else {
        cout << "员工进货记录更新失败！" << endl;
    }
    free();
}

//向药房补货
void to_mediout() {
    //string Bid, Bsum;
    apply_mid();
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";//第一句要执行的SQL语句
    string str2 = "SELECT Mcount,Mid FROM Medi Where Mid='";//第二句要执行的SQL语句
    string str3 = str2 + to_string(mediid) + "'";

    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        SQLCHAR str_1[10],str_2[10];
        SQLLEN len_str1,len_str2;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str_1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str_2, 10, &len_str2);
        }
        string str_num((const char*)str_1);
        string str_id((const char*)str_2);
        str_num.erase(str_num.find_last_not_of(" ") + 1);
        str_id.erase(str_id.find_last_not_of(" ") + 1);

        istringstream ss(str_num);
        ss >> num;

        if (num >= medinum) {
            cout << "库存充足，可以补货";
            flag_medi = 1;
        }
        else {
            cout << "库存不足，请调整，仓库当前库存数量：" << str_num;
            flag_medi = 0;
        }
    }
    free();
    if (flag_medi == 1) {
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
        //更新仓库药品数量
        int num_update;
        num_update = num - medinum;
        string str1 = "use Medicine";
        string str2 = "UPDATE Medi SET Mcount = " + to_string(num_update) + " Where Medi.Mid = " + to_string(mediid);
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
        flag_mediout = 1;
    }
    else {
        flag_mediout = 0;
    }
    free();
}

//向药房补货新
void to_mediout_new() {
    int num_apply=0;
    int number=0;
    string id_apply;
    string state_apply;
    //查看申请单
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";//第一句要执行的SQL语句
    string str2 = "SELECT* FROM Medi_apply";//第二句要执行的SQL语句

    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
  
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str_1[10], str_2[20], str_3[10],str_4[20];
        SQLLEN len_str1, len_str2, len_str3,len_str4;
        cout << "申请药品编号" << "    " << "申请药品名称" << "    " << "申请药品数量" << "    " << "申请状态" << endl;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str_1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str_2, 20, &len_str2);
            SQLGetData(hstmt, 3, SQL_C_CHAR, str_3, 10, &len_str3);
            SQLGetData(hstmt, 4, SQL_C_CHAR, str_4, 20, &len_str4);
            printf("%s\t%s\t%s\t%s\n", str_1, str_2, str_3,str_4);
        }
        string str_id((const char*)str_1);
        string str_num((const char*)str_3);
        string str_apply((const char*)str_4);
        str_id.erase(str_id.find_last_not_of(" ") + 1);
        str_num.erase(str_num.find_last_not_of(" ") + 1);
        str_apply.erase(str_apply.find_last_not_of(" ") + 1);
        id_apply = str_id;
        state_apply = str_apply;
        istringstream ss(str_num);
        ss >> num_apply;
    }
    free();

    //检查库存是否充足
    if (state_apply == "未完成") {
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
        string str3 = "use Medicine";//第一句要执行的SQL语句
        string str4 = "SELECT Mcount,Mid FROM Medi Where Mid='";//第二句要执行的SQL语句
        string str5 = str4 + id_apply + "'";


        ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str5.c_str(), SQL_NTS);

        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            SQLCHAR str_4[10], str_5[10];
            SQLLEN len_str4, len_str5;
            while (SQLFetch(hstmt) != SQL_NO_DATA)
            {
                SQLGetData(hstmt, 1, SQL_C_CHAR, str_4, 10, &len_str4);
                SQLGetData(hstmt, 2, SQL_C_CHAR, str_5, 10, &len_str5);
            }
            string sstr_num((const char*)str_4);
            string sstr_id((const char*)str_5);
            sstr_num.erase(sstr_num.find_last_not_of(" ") + 1);
            sstr_id.erase(sstr_id.find_last_not_of(" ") + 1);

            istringstream ss(sstr_num);
            ss >> number;

            if (number >= num_apply) {
                cout << "库存充足，可以补货" << endl;
                flag_medi = 1;
            }
            else {
                cout << "库存不足，请调整，仓库当前库存数量：" << number << endl;
                tip();
                flag_medi = 0;
            }
        }
        free();
    }
    else {
        cout << "没有需要处理的申请" << endl;
    }

    if (flag_medi == 1) {
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
        //更新仓库药品数量
        int num_update;
        num_update = number - num_apply;
        string str6 = "use Medicine";
        string str7 = "UPDATE Medi SET Mcount = " + to_string(num_update) + " Where Medi.Mid = " + id_apply;
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str6.c_str(), SQL_NTS);
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str7.c_str(), SQL_NTS);
        flag_mediout = 1;
        update_state();
        cout << "补货完成"<<endl;
    }
    else {
        flag_mediout = 0;
    }

    free();
}

//在申请单中提示库存不足
void tip() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str6 = "use Medicine";
    string str7 = "UPDATE Medi_apply SET Mstate = '库存不足,请重新申请'" ;
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str6.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str7.c_str(), SQL_NTS);
}

//更改申请单完成状态
void update_state() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str6 = "use Medicine";
    string str7 = "UPDATE Medi_apply SET Mstate = '已完成'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str6.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str7.c_str(), SQL_NTS);
}



/***********************药房工作人员相关操作**************************/

//检查药房所有药品库存
void print_mediout_all()
{
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT Mid, Mname, Mnum, Mpriceout FROM Mediout";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        cout << "药品编号" << "    " << "药品名称" << "    " << "药品数量" << "     " << "药品单价" << endl;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmt, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmt, 4, SQL_C_CHAR, str4, 30, &len_str4);
            printf("%s\t%s\t%s\t%s\n", str1, str2, str3, str4);
        }
    }
    free();
}

//检查是否有该药品
void print_mediout()
{
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT Mid, Mname, Mnum, Mpriceout FROM Mediout where Mid='";
    cout << "请输入药品编号" << endl;
    //string Sno;
    cin >> medi_outno;
    string str3 = str2 + medi_outno + "'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        cout << "药品编号" << "    " << "药品名称" << "    " << "药品数量" << "     " << "药品单价" << endl;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmt, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmt, 4, SQL_C_CHAR, str4, 30, &len_str4);
            printf("%s\t%s\t%s\t%s\n", str1, str2, str3, str4);
        }

        string outname((const char*)str2);
        string str_num((const char*)str3);
        str_num.erase(str_num.find_last_not_of(" ") + 1);
        outname.erase(outname.find_last_not_of(" ") + 1);
        medi_outname = outname;
        istringstream ss(str_num);
        ss >> mediout_num;
    }
    free();
}

//卖药
void sold_mid() {
    print_mediout();
    cout << "请输入要购买的数量：";
    int  num_sold;
    cin >> num_sold;
     if (mediout_num >= num_sold ) {
         ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
         //更新药房药品数量
         int num_update;
         num_update = mediout_num - num_sold;
         string str1 = "use Medicine";
         string str2 = "UPDATE Mediout SET Mnum = " + to_string(num_update) + " Where Mediout.Mid = " + medi_outno;

         ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
         ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
         free();

         //更新出售记录单
         ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
         cout << "请依次输入工号、售出时间" << endl;
         string Wid, sold_date;
         cin >> Wid >> sold_date;
         string str3 = "use Medicine";
         string str4 = "INSERT INTO Medi_sold (Mid,Mname,Mnumout,Mworkerout,Mtimeout) Values('" 
             + medi_outno + "','" + medi_outname + "','" + to_string(num_sold) + "','" + Wid + "','" + sold_date +"')";
         ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
         ret = SQLExecDirect(hstmt, (SQLCHAR*)str4.c_str(), SQL_NTS);
    }
     else {
         cout << "数量不足，请重新填写购买数量";
     }
    free();
}

//申请补货
void apply_mid() {
    cout << "请依次输入需要补充的药品编号、药品数量" << endl;
    cin >> mediid >> medinum;
}

//申请补货新
void apply_mid_new() {
    cout << "请依次输入需要补充的药品编号、药品名称、药品数量" << endl;
    string Mid, Mname, Mcount;
    string state = "未完成";
    cin >> Mid >> Mname >> Mcount;
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";//第一句要执行的SQL语句
    string str2 = "insert into Medi_apply values ('";//第二句要执行的SQL语句
    string str3 = str2 + Mid + "','" + Mname + "','" + Mcount +"','"+state+"')";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "申请成功！" << endl;
    }
    else {
        cout << "申请失败！" << endl;
    }
    free();
}

//修改驳回申请
void update_apply() {
    string id_apply;
    //查看被驳回申请
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";//第一句要执行的SQL语句
    string str2 = "SELECT* FROM Medi_apply Where Mstate = '库存不足,请重新申请'";//第二句要执行的SQL语句

    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str_1[10], str_2[20], str_3[10], str_4[20];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        cout << "申请药品编号" << "    " << "申请药品名称" << "    " << "申请药品数量" << "    " << "申请状态" << endl;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str_1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str_2, 20, &len_str2);
            SQLGetData(hstmt, 3, SQL_C_CHAR, str_3, 10, &len_str3);
            SQLGetData(hstmt, 4, SQL_C_CHAR, str_4, 10, &len_str4);
            printf("%s\t%s\t%s\t%s\n", str_1, str_2, str_3, str_4);
        }
        string str_id((const char*)str_1);
        str_id.erase(str_id.find_last_not_of(" ") + 1);
        id_apply = str_id;
    }
    free();

    //修改申请数量及状态
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string update_count;
    cout << "请重新输入申请数量" << endl;
    cin >> update_count;
    string str3 = "use Medicine";
    string str4 = "UPDATE Medi_apply SET Mcount = " + update_count +", Mstate = '未完成'  Where Medi_apply.Mid = '" + id_apply+"'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str4.c_str(), SQL_NTS);
    cout << "修改成功";
    free();

}

//从药房补货
void update_mediout() {
    if (flag_mediout == 1) {
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
        //更新药房药品数量
        int num_update;
        num_update = mediout_num + medinum;
        string str1 = "use Medicine";
        string str2 = "UPDATE Mediout SET Mnum = " + to_string(num_update) + " Where Mediout.Mid = " + to_string(mediid);
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
        flag_mediout = 0;
    }
    else {
        cout <<  "仓库库存不足，请重新申请";
    }
    free();
}

//从药房补货新
void update_mediout_new() {
    string id_apply;
    string state_apply;
    string num_apply;
    //查看申请完成状态
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";//第一句要执行的SQL语句
    string str2 = "SELECT* FROM Medi_apply Where Mstate = '已完成'";//第二句要执行的SQL语句

    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str_1[10], str_2[20], str_3[10], str_4[20];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        cout << "申请药品编号" << "    " << "申请药品名称" << "    " << "申请药品数量" << "    " << "申请状态" << endl;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str_1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str_2, 20, &len_str2);
            SQLGetData(hstmt, 3, SQL_C_CHAR, str_3, 10, &len_str3);
            SQLGetData(hstmt, 4, SQL_C_CHAR, str_4, 10, &len_str4);
            printf("%s\t%s\t%s\t%s\n", str_1, str_2, str_3, str_4);
        }
        string str_id((const char*)str_1);
        str_id.erase(str_id.find_last_not_of(" ") + 1);
        id_apply = str_id;

        string str_num((const char*)str_3);
        str_num.erase(str_num.find_last_not_of(" ") + 1);
        num_apply = str_num;
        int apply_num = 0;
        istringstream ss(num_apply);
        ss >> apply_num;

        string str_state((const char*)str_4);
        str_state.erase(str_state.find_last_not_of(" ") + 1);
        state_apply = str_state;
    }
    free();

    //修改药房库存
    if (state_apply == "已完成") {
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
        //更新药房药品数量
        string str1 = "use Medicine";
        string str2 = "UPDATE Mediout SET Mnum =( Mnum + " + num_apply + ") Where Mediout.Mid = " + id_apply;
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
        flag_mediout = 0;
        free();
    }
    //删除申请
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str3 = "use Medicine";//第一句要执行的SQL语句
    string str4 = "delete from Medi_apply Where Mid = '"+ id_apply+"'";//第二句要执行的SQL语句

    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str4.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "删除成功！" << endl;
    }
    else {
        cout << "删除失败！" << endl;
    }
    free();
}



/***********************人事相关操作**************************/
//查看员工信息
void print_workers() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Workers";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30], str5[40], str6[50], str7[60];
        SQLLEN len_str1, len_str2, len_str3, len_str4, len_str5, len_str6, len_str7;
        cout << "工号" << "    " << "员工姓名" << "    " << "员工年龄" << "     " << "工作地点" << "     " << "工资" << "     " << "账号" << "     " << "密码" << endl;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmt, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmt, 4, SQL_C_CHAR, str4, 30, &len_str4);
            SQLGetData(hstmt, 5, SQL_C_CHAR, str5, 10, &len_str5);
            SQLGetData(hstmt, 6, SQL_C_CHAR, str6, 10, &len_str6);
            SQLGetData(hstmt, 7, SQL_C_CHAR, str7, 10, &len_str7);
            printf("%s\t%s\t%s\t%s\t%s\t%s\t%s\n", str1, str2, str3, str4, str5, str6, str7);
        }
    }
    free();
}

//查看总盈利
void print_profit() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT SUM(profit) AS total from Profit";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10];
        SQLLEN len_str1;
        cout << "总盈利" << endl;
        while (SQLFetch(hstmt) != SQL_NO_DATA) {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str1, 10, &len_str1);
            printf("%s\n", str1);
        }
    }
    free();
}

//查看药房员工卖药情况
void view_medi_sold() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Medi_sold";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30], str5[40];
        SQLLEN len_str1, len_str2, len_str3, len_str4, len_str5;
        cout << "药品编号" << "    " << "药品名称" << "    " << "卖出数量" << "     " << "卖出人工号" << "     " << "卖出时间" << endl;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmt, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmt, 4, SQL_C_CHAR, str4, 30, &len_str4);
            SQLGetData(hstmt, 5, SQL_C_CHAR, str5, 20, &len_str5);
            printf("%s\t%s\t%s\t%s\t%s\n", str1, str2, str3, str4, str5);
        }
    }
    free();
}

//查看仓库员工进货情况
void view_medi_inlist() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Medi_inlist";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30], str5[40];
        SQLLEN len_str1, len_str2, len_str3, len_str4, len_str5;
        cout << "药品编号" << "    " << "药品名称" << "    " << "进货时间" << "     " << "进货人工号" << "     " << "进货数量" << endl;
        while (SQLFetch(hstmt) != SQL_NO_DATA)
        {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str1, 10, &len_str1);
            SQLGetData(hstmt, 2, SQL_C_CHAR, str2, 12, &len_str2);
            SQLGetData(hstmt, 3, SQL_C_CHAR, str3, 20, &len_str3);
            SQLGetData(hstmt, 4, SQL_C_CHAR, str4, 30, &len_str4);
            SQLGetData(hstmt, 5, SQL_C_CHAR, str5, 10, &len_str5);
            printf("%s\t%s\t%s\t%s\t%s\n", str1, str2, str3, str4, str5);
        }
    }
    free();
}

//招聘
void insert_worker() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";//第一句要执行的SQL语句
    string str2 = "insert into Workers (Wid, Wname, Wage, Wplace) values ('";//第二句要执行的SQL语句
    cout << "请依次输入工号、员工姓名、员工年龄、员工工作地点" << endl;
    cout << "例：145 齐 3000 store" << endl;
    string Bno, Bname, Bage, Bplace;
    cin >> Bno >> Bname >> Bage >> Bplace ;
    string str3 = str2 + Bno + "','" + Bname + "','" + Bage + "','" + Bplace + "')";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "员工添加成功！" << endl;
    }
    else {
        cout << "员工添加失败！" << endl;
    }
    free();
}

//解雇
void delete_worker() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string str1 = "use Medicine";//第一句要执行的SQL语句
    string str2 = "delete from Workers Where Wid = '";//第二句要执行的SQL语句
    cout << "请依次输入解雇人工号" << endl;
    string Bno;
    cin >> Bno ;
    string str3 = str2 + Bno +"'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "解雇成功！" << endl;
    }
    else {
        cout << "解雇失败！" << endl;
    }
    free();
}

//调整工资
void update_salary() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请句柄
    string Wid, new_salary;
    cout << "请依次输入员工工号、更新后的工资" << endl;
    cin >> Wid >> new_salary;

    string str1 = "use Medicine";
    string str2 = "UPDATE Workers SET Wsalary = " + new_salary + " Where Workers.Wid = " + Wid;

    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    cout << "更新成功！" << endl;
    free();
}



int main()
{
    connect();

    //login();
    //reg();

    //print_medi();
    //print_medi_all();
    //update_medi();
    //to_mediout();

    //to_mediout_new();

    //print_mediout();
    //print_mediout_all();
    //sold_mid();
    //apply_mid();
     
    //apply_mid_new();
    //update_apply();
    //update_mediout();
    
    //update_mediout_new();
     
    //print_workers();
    //print_profit();
    //view_medi_sold();
    //view_medi_inlist();
    //insert_worker();
    //delete_worker();
    //update_salary();

    //apply_mid_new();
    //to_mediout_new();
    //update_apply();
    //to_mediout_new();
    //update_mediout_new();

    return 0;
}


