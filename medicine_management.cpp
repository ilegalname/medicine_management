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


int medinum = 0;   //��������
int mediid = 0;   //����id
int num = 0;   //�ֿ���
int mediout_num; //ҩ�����
int flag_medi;  //�ֿ��Ƿ���Բ���
int flag_mediout;  //ҩ���Ƿ���Բ���
string medi_outno;  //����id
string medi_outname;   //��������

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



/***********************���ݿ��������ͷ�**************************/
//free
void free()
{
    SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//�ͷ����
    SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//�ͷ�����
    SQLFreeHandle(SQL_HANDLE_ENV, henv);//�ͷŻ���
}

//�������ݿ�
void connect()//���ݿ����Ӻ���
{
    SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);//���뻷��
    SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//���û���
    SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);//�������ݿ�����
    ret = SQLConnect(hdbc, (SQLTCHAR*)("sql"), SQL_NTS, (SQLTCHAR*)("sa"), SQL_NTS, (SQLTCHAR*)("sa1234"), SQL_NTS);
    if ((ret == SQL_SUCCESS) || (ret == SQL_SUCCESS_WITH_INFO))
    {
        cout << "���ݿ����ӳɹ�!" << endl;
    }
    else
    {
        cout << "���ݿ�����ʧ�ܣ�" << endl;
    }
}



/***********************Ա��ע�ἰ��¼�˺�**************************/
//��¼
void login() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string log_id, log_key, worker_id;
    cout << "------����ҩƷ�����ϵͳ------" << endl;
    cout << "�������û���:";
    cin >> log_id;
    cout << "����������:";
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
                cout << "��¼�ɹ���" << endl;
            }
            else {
                cout << "�����������������" << endl;
            }
        }
        else {
            cout << "�û��������ڣ���ע��" << endl;
            reg();
        }
    }
    free();
}

//ע��
void reg() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string log_id, log_key, log_kkey,worker_id;
    cout << "���������Ĺ��ţ�";
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
            cout << "����ע��" << endl;
            cout << "�������û���:";
            cin >> log_id;
            cout << "����������";
            cin >> log_key;
            cout << "��ȷ������";
            cin >> log_kkey;
            free();
            if (log_key == log_kkey) {
                ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
                string str1 = "use Medicine";
                //string str2 = "UPDATE Workers SET Mnum = " + to_string(num_update) + " Where Mediout.Mid = " + medi_outno;

                ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
                ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
                free();
            }
            else {
               // cout << "�������벻һ�£�����������";
            }
        }
    }


}



/***********************�ֿ⹤����Ա��ز���**************************/
//�ֿ����Ƿ��и�ҩƷ
void print_medi()
{
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";
    string str2 = "SELECT*FROM Medi where Mid='";
    cout << "������ҩƷ���" << endl;
    string Sno;
    cin >> Sno;
    string str3 = str2 + Sno + "'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3,len_str4;
        cout << "ҩƷ���" << "    " << "ҩƷ����" << "    " << "ҩƷ����"  <<"     " << "ҩƷ����" << endl;
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

//���ֿ�����ҩƷ���
void print_medi_all()
{
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";
    string str2 = "SELECT*FROM Medi";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        cout << "ҩƷ���" << "    " << "ҩƷ����" << "    " << "ҩƷ����" << "     " << "ҩƷ����" << endl;
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

//�����嵥+���²ֿ�ҩƷ���+����Ա��������¼
void update_medi()
{   
    //���¿��
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";//��һ��Ҫִ�е�SQL���
    string str2 = "insert into Medi values ('";//�ڶ���Ҫִ�е�SQL���
    cout << "����������ҩƷ��š�ҩƷ���ơ�ҩƷ������ҩƷ���ۡ����š�����ʱ��" << endl;
    cout << "����55 DEA 12 56 66 2024-9-6" << endl;
    string Bno, Bname, Bsum, Bprice, Wid, Bdate;
    cin >> Bno >> Bname >> Bsum >> Bprice >> Wid >> Bdate;
    string str3 = str2 + Bno + "','" + Bname + "','" + Bsum + "','" + Bprice + "')";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "ҩƷ�ϼܳɹ���" << endl;
    }
    else {
        cout << "ҩƷ�ϼ�ʧ�ܣ�" << endl;
    }
    free();

    //���¼�¼
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str4 = "use Medicine";//��һ��Ҫִ�е�SQL���
    string str5 = "insert into Medi_inlist values ('";//�ڶ���Ҫִ�е�SQL���
    string str6 = str5 + Bno + "','" + Bname + "','" + Bdate + "','" + Wid + "','" + Bsum + "')";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str4.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str6.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "Ա��������¼���³ɹ���" << endl;
    }
    else {
        cout << "Ա��������¼����ʧ�ܣ�" << endl;
    }
    free();
}

//��ҩ������
void to_mediout() {
    //string Bid, Bsum;
    apply_mid();
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";//��һ��Ҫִ�е�SQL���
    string str2 = "SELECT Mcount,Mid FROM Medi Where Mid='";//�ڶ���Ҫִ�е�SQL���
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
            cout << "�����㣬���Բ���";
            flag_medi = 1;
        }
        else {
            cout << "��治�㣬��������ֿ⵱ǰ���������" << str_num;
            flag_medi = 0;
        }
    }
    free();
    if (flag_medi == 1) {
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
        //���²ֿ�ҩƷ����
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

//��ҩ��������
void to_mediout_new() {
    int num_apply=0;
    int number=0;
    string id_apply;
    string state_apply;
    //�鿴���뵥
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";//��һ��Ҫִ�е�SQL���
    string str2 = "SELECT* FROM Medi_apply";//�ڶ���Ҫִ�е�SQL���

    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
  
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str_1[10], str_2[20], str_3[10],str_4[20];
        SQLLEN len_str1, len_str2, len_str3,len_str4;
        cout << "����ҩƷ���" << "    " << "����ҩƷ����" << "    " << "����ҩƷ����" << "    " << "����״̬" << endl;
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

    //������Ƿ����
    if (state_apply == "δ���") {
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
        string str3 = "use Medicine";//��һ��Ҫִ�е�SQL���
        string str4 = "SELECT Mcount,Mid FROM Medi Where Mid='";//�ڶ���Ҫִ�е�SQL���
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
                cout << "�����㣬���Բ���" << endl;
                flag_medi = 1;
            }
            else {
                cout << "��治�㣬��������ֿ⵱ǰ���������" << number << endl;
                tip();
                flag_medi = 0;
            }
        }
        free();
    }
    else {
        cout << "û����Ҫ���������" << endl;
    }

    if (flag_medi == 1) {
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
        //���²ֿ�ҩƷ����
        int num_update;
        num_update = number - num_apply;
        string str6 = "use Medicine";
        string str7 = "UPDATE Medi SET Mcount = " + to_string(num_update) + " Where Medi.Mid = " + id_apply;
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str6.c_str(), SQL_NTS);
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str7.c_str(), SQL_NTS);
        flag_mediout = 1;
        update_state();
        cout << "�������"<<endl;
    }
    else {
        flag_mediout = 0;
    }

    free();
}

//�����뵥����ʾ��治��
void tip() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str6 = "use Medicine";
    string str7 = "UPDATE Medi_apply SET Mstate = '��治��,����������'" ;
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str6.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str7.c_str(), SQL_NTS);
}

//�������뵥���״̬
void update_state() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str6 = "use Medicine";
    string str7 = "UPDATE Medi_apply SET Mstate = '�����'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str6.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str7.c_str(), SQL_NTS);
}



/***********************ҩ��������Ա��ز���**************************/

//���ҩ������ҩƷ���
void print_mediout_all()
{
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";
    string str2 = "SELECT Mid, Mname, Mnum, Mpriceout FROM Mediout";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        cout << "ҩƷ���" << "    " << "ҩƷ����" << "    " << "ҩƷ����" << "     " << "ҩƷ����" << endl;
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

//����Ƿ��и�ҩƷ
void print_mediout()
{
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";
    string str2 = "SELECT Mid, Mname, Mnum, Mpriceout FROM Mediout where Mid='";
    cout << "������ҩƷ���" << endl;
    //string Sno;
    cin >> medi_outno;
    string str3 = str2 + medi_outno + "'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        cout << "ҩƷ���" << "    " << "ҩƷ����" << "    " << "ҩƷ����" << "     " << "ҩƷ����" << endl;
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

//��ҩ
void sold_mid() {
    print_mediout();
    cout << "������Ҫ�����������";
    int  num_sold;
    cin >> num_sold;
     if (mediout_num >= num_sold ) {
         ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
         //����ҩ��ҩƷ����
         int num_update;
         num_update = mediout_num - num_sold;
         string str1 = "use Medicine";
         string str2 = "UPDATE Mediout SET Mnum = " + to_string(num_update) + " Where Mediout.Mid = " + medi_outno;

         ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
         ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
         free();

         //���³��ۼ�¼��
         ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);
         cout << "���������빤�š��۳�ʱ��" << endl;
         string Wid, sold_date;
         cin >> Wid >> sold_date;
         string str3 = "use Medicine";
         string str4 = "INSERT INTO Medi_sold (Mid,Mname,Mnumout,Mworkerout,Mtimeout) Values('" 
             + medi_outno + "','" + medi_outname + "','" + to_string(num_sold) + "','" + Wid + "','" + sold_date +"')";
         ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
         ret = SQLExecDirect(hstmt, (SQLCHAR*)str4.c_str(), SQL_NTS);
    }
     else {
         cout << "�������㣬��������д��������";
     }
    free();
}

//���벹��
void apply_mid() {
    cout << "������������Ҫ�����ҩƷ��š�ҩƷ����" << endl;
    cin >> mediid >> medinum;
}

//���벹����
void apply_mid_new() {
    cout << "������������Ҫ�����ҩƷ��š�ҩƷ���ơ�ҩƷ����" << endl;
    string Mid, Mname, Mcount;
    string state = "δ���";
    cin >> Mid >> Mname >> Mcount;
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";//��һ��Ҫִ�е�SQL���
    string str2 = "insert into Medi_apply values ('";//�ڶ���Ҫִ�е�SQL���
    string str3 = str2 + Mid + "','" + Mname + "','" + Mcount +"','"+state+"')";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "����ɹ���" << endl;
    }
    else {
        cout << "����ʧ�ܣ�" << endl;
    }
    free();
}

//�޸Ĳ�������
void update_apply() {
    string id_apply;
    //�鿴����������
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";//��һ��Ҫִ�е�SQL���
    string str2 = "SELECT* FROM Medi_apply Where Mstate = '��治��,����������'";//�ڶ���Ҫִ�е�SQL���

    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str_1[10], str_2[20], str_3[10], str_4[20];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        cout << "����ҩƷ���" << "    " << "����ҩƷ����" << "    " << "����ҩƷ����" << "    " << "����״̬" << endl;
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

    //�޸�����������״̬
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string update_count;
    cout << "������������������" << endl;
    cin >> update_count;
    string str3 = "use Medicine";
    string str4 = "UPDATE Medi_apply SET Mcount = " + update_count +", Mstate = 'δ���'  Where Medi_apply.Mid = '" + id_apply+"'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str4.c_str(), SQL_NTS);
    cout << "�޸ĳɹ�";
    free();

}

//��ҩ������
void update_mediout() {
    if (flag_mediout == 1) {
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
        //����ҩ��ҩƷ����
        int num_update;
        num_update = mediout_num + medinum;
        string str1 = "use Medicine";
        string str2 = "UPDATE Mediout SET Mnum = " + to_string(num_update) + " Where Mediout.Mid = " + to_string(mediid);
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
        flag_mediout = 0;
    }
    else {
        cout <<  "�ֿ��治�㣬����������";
    }
    free();
}

//��ҩ��������
void update_mediout_new() {
    string id_apply;
    string state_apply;
    string num_apply;
    //�鿴�������״̬
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";//��һ��Ҫִ�е�SQL���
    string str2 = "SELECT* FROM Medi_apply Where Mstate = '�����'";//�ڶ���Ҫִ�е�SQL���

    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str_1[10], str_2[20], str_3[10], str_4[20];
        SQLLEN len_str1, len_str2, len_str3, len_str4;
        cout << "����ҩƷ���" << "    " << "����ҩƷ����" << "    " << "����ҩƷ����" << "    " << "����״̬" << endl;
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

    //�޸�ҩ�����
    if (state_apply == "�����") {
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
        //����ҩ��ҩƷ����
        string str1 = "use Medicine";
        string str2 = "UPDATE Mediout SET Mnum =( Mnum + " + num_apply + ") Where Mediout.Mid = " + id_apply;
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
        ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
        flag_mediout = 0;
        free();
    }
    //ɾ������
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str3 = "use Medicine";//��һ��Ҫִ�е�SQL���
    string str4 = "delete from Medi_apply Where Mid = '"+ id_apply+"'";//�ڶ���Ҫִ�е�SQL���

    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str4.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "ɾ���ɹ���" << endl;
    }
    else {
        cout << "ɾ��ʧ�ܣ�" << endl;
    }
    free();
}



/***********************������ز���**************************/
//�鿴Ա����Ϣ
void print_workers() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Workers";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30], str5[40], str6[50], str7[60];
        SQLLEN len_str1, len_str2, len_str3, len_str4, len_str5, len_str6, len_str7;
        cout << "����" << "    " << "Ա������" << "    " << "Ա������" << "     " << "�����ص�" << "     " << "����" << "     " << "�˺�" << "     " << "����" << endl;
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

//�鿴��ӯ��
void print_profit() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";
    string str2 = "SELECT SUM(profit) AS total from Profit";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10];
        SQLLEN len_str1;
        cout << "��ӯ��" << endl;
        while (SQLFetch(hstmt) != SQL_NO_DATA) {
            SQLGetData(hstmt, 1, SQL_C_CHAR, str1, 10, &len_str1);
            printf("%s\n", str1);
        }
    }
    free();
}

//�鿴ҩ��Ա����ҩ���
void view_medi_sold() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Medi_sold";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30], str5[40];
        SQLLEN len_str1, len_str2, len_str3, len_str4, len_str5;
        cout << "ҩƷ���" << "    " << "ҩƷ����" << "    " << "��������" << "     " << "�����˹���" << "     " << "����ʱ��" << endl;
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

//�鿴�ֿ�Ա���������
void view_medi_inlist() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";
    string str2 = "SELECT* FROM Medi_inlist";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
    {
        SQLCHAR str1[10], str2[12], str3[20], str4[30], str5[40];
        SQLLEN len_str1, len_str2, len_str3, len_str4, len_str5;
        cout << "ҩƷ���" << "    " << "ҩƷ����" << "    " << "����ʱ��" << "     " << "�����˹���" << "     " << "��������" << endl;
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

//��Ƹ
void insert_worker() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";//��һ��Ҫִ�е�SQL���
    string str2 = "insert into Workers (Wid, Wname, Wage, Wplace) values ('";//�ڶ���Ҫִ�е�SQL���
    cout << "���������빤�š�Ա��������Ա�����䡢Ա�������ص�" << endl;
    cout << "����145 �� 3000 store" << endl;
    string Bno, Bname, Bage, Bplace;
    cin >> Bno >> Bname >> Bage >> Bplace ;
    string str3 = str2 + Bno + "','" + Bname + "','" + Bage + "','" + Bplace + "')";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "Ա����ӳɹ���" << endl;
    }
    else {
        cout << "Ա�����ʧ�ܣ�" << endl;
    }
    free();
}

//���
void delete_worker() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string str1 = "use Medicine";//��һ��Ҫִ�е�SQL���
    string str2 = "delete from Workers Where Wid = '";//�ڶ���Ҫִ�е�SQL���
    cout << "�������������˹���" << endl;
    string Bno;
    cin >> Bno ;
    string str3 = str2 + Bno +"'";
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str3.c_str(), SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "��ͳɹ���" << endl;
    }
    else {
        cout << "���ʧ�ܣ�" << endl;
    }
    free();
}

//��������
void update_salary() {
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//������
    string Wid, new_salary;
    cout << "����������Ա�����š����º�Ĺ���" << endl;
    cin >> Wid >> new_salary;

    string str1 = "use Medicine";
    string str2 = "UPDATE Workers SET Wsalary = " + new_salary + " Where Workers.Wid = " + Wid;

    ret = SQLExecDirect(hstmt, (SQLCHAR*)str1.c_str(), SQL_NTS);
    ret = SQLExecDirect(hstmt, (SQLCHAR*)str2.c_str(), SQL_NTS);
    cout << "���³ɹ���" << endl;
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


