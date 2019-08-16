#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include <conio.h>
#include <clocale>
/* SQL ODBC */
#include <sql.h>
#include <sqlext.h>
#define df_DB_IP L"127.0.0.1"
#define df_DB_ID L"root"
#define df_DB_PASS L"1234"
#define df_DB_NAME L"game_schema"
#define df_DB_PORT 3306

SQLHENV		g_hEnv;	
SQLHDBC		g_hDbc;
SQLHSTMT	g_hStmt;

SQLRETURN	MYSQLConnect(const WCHAR * szODBCIP, const WCHAR * szODBCName, const WCHAR * szODBCID, const WCHAR * szODBCPW);
void		MYSQLDisconnect();

int main()
{
	WCHAR		szStat[10];
	WCHAR		szMsg[512];
	SQLSMALLINT	iMsglen;
	int			iNativeErr;

	setlocale(LC_ALL, "");

	/**********************************************************/
	// DB ����
	/**********************************************************/
	SQLRETURN iRetval = MYSQLConnect(df_DB_IP, df_DB_NAME, df_DB_ID, df_DB_PASS);
	if (iRetval != 0)
	{
		SQLGetDiagRec(SQL_HANDLE_DBC, g_hDbc, 1, (SQLTCHAR*)szStat, (SQLINTEGER*)&iNativeErr, (SQLTCHAR*)szMsg, 1024, &iMsglen);
		wprintf(L"%s\n", szMsg);
		return iRetval;
	}
	
	/**********************************************************/
	// ���� ����
	/**********************************************************/
	SQLCHAR		BuffNo[40], BuffID[40], BuffPass[1000];
	SQLLEN		lBuffNo = 0, lBuffID = 0, lBuffPass = 0;
	// ���ε� (select������ ������ ����� �������� ���� + SQL ������ ����)
	// * SQLBindCol(����ڵ�, �÷���ȣ, �ش��÷��� ����������, ����, ���� '����', )  
	SQLBindCol(g_hStmt, 1, SQL_C_TCHAR, BuffNo, sizeof(BuffNo), (SQLLEN*)&lBuffNo);
	SQLBindCol(g_hStmt, 2, SQL_C_TCHAR, BuffID, sizeof(BuffID), (SQLLEN*)&lBuffID);
	SQLBindCol(g_hStmt, 3, SQL_C_TCHAR, BuffPass, sizeof(BuffPass), (SQLLEN*)&lBuffPass);

	///SQLExecDirect(hStmt, (SQLTCHAR*)"INSERT INTO Member(ID,Pass,Name) VALUES('myID', '2222', 'long'", SQL_NTS);
	SQLExecDirect(g_hStmt, (SQLTCHAR*)L"SELECT * FROM account", SQL_NTS);;

	/**********************************************************/
	// ��� ���
	/**********************************************************/
	// * �÷��� ���� �������ִ� �Լ� : Ŭ����ȭ�Ҷ� ���. ������ ������
	//	SQLNumResultCols(hStmt, (int)&�÷���);
	// * �÷� �̸� ������
	//	SQLDescribeCol(hStmt, �÷���ȣ, Buffer, sizeof(Buffer), NULL,NULL,NULL,NULL,NULL)
	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		wprintf(L"%s	%s	%s\n", BuffNo, BuffID, BuffPass);
		SQLGetDiagRec(SQL_HANDLE_STMT, g_hStmt, 1, (SQLTCHAR*)szStat, (SQLINTEGER*)&iNativeErr, (SQLTCHAR*)szMsg, 1024, &iMsglen);
		//wprintf(L"%s\n", szMsg);

		/*
		// ���� ������ ����� �ٽ� ������ �������ϴ� ���
		SQLHSTMT hStmt2;
		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt2) != SQL_SUCCESS) return false;
		hResult = SQLBindCol(hStmt2, 1, SQL_C_CHAR, BuffPass, 8, (SQLLEN*)&lBuffID);
		hResult = SQLExecDirect(hStmt2, (SQLTCHAR*)"SELECT * FROM tblUser", SQL_NTS);
		SQLGetDiagRec(SQL_HANDLE_STMT, hStmt2, 1, (SQLTCHAR*)szStat, (SQLINTEGER*)&iNativeErr, (SQLTCHAR*)szMsg, 1024, &iMsglen);
		wprintf(L"%s\n", szMsg);

		while (SQLFetch(hStmt2) != SQL_NO_DATA)
		{
		wprintf(L"ID:%s, Pass:%s, Nick:%s\n", BuffID, BuffPass, BuffNick);
		}
		SQLCloseCursor(hStmt2);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt2);
		*/
	}

	/**********************************************************/
	// DB ���� ����
	/**********************************************************/
	MYSQLDisconnect();

	return 0;
}

SQLRETURN MYSQLConnect(const WCHAR * szODBCIP, const WCHAR * szODBCName, const WCHAR * szODBCID, const WCHAR * szODBCPW)
{
	SQLRETURN iRetval = SQL_SUCCESS;

	////////////////////////////////////////////////////////////////
	// �ڵ� �Ҵ�
	//
	////////////////////////////////////////////////////////////////
	// 1. ȯ�� �ڵ� �Ҵ�
	// * SQLAllocHandle() : ȯ��, ����, ��ɹ� �ڵ� �Ҵ�
	if (iRetval = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv) < 0) // 0 = SQL_SUCCESS, 1 = SQL_SUCCESS_WITH_INFO
		return iRetval;

	// 2. ODBC ����̹� ���� ����
	// * SQLSetEnvAttr() : ȯ�� ���� Ư�� ����
	if (iRetval = SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0) < 0)
		return iRetval;

	// 3. ���� �ڵ� �Ҵ�
	if (iRetval = SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc) < 0)
		return iRetval;


	////////////////////////////////////////////////////////////////
	// DBMS ����
	//
	////////////////////////////////////////////////////////////////
	// 3. DB ���� ���1
	// * SQLConnect() : ����� DSN�� ���Ͽ� DB ����
	// - ODBC ����̹� DSN ���� : ������ ~ �������� ~ �����Ϳ���(ODBC) ~ ����� DSN ��
	//if (iRetval = SQLConnect(g_hDbc, (SQLTCHAR*)"connect_gameschema", SQL_NTS, (SQLTCHAR*)szODBCID, SQL_NTS, (SQLTCHAR*)szODBCPW, SQL_NTS) < 0)
	//	return iRetval;
	
	// 3. DB ���� ���2
	// * SQLDriverConnect() : ODBC Driver�� ���� �����Ͽ� DB ����
	// - ODBC ����̹��� ��ȸ : ������ ~ �������� ~ �����Ϳ���(ODBC) ~ ����̹���
	WCHAR szConIn[255] = L"\0";
	swprintf_s(szConIn, L"DRIVER={MySQL ODBC 5.3 Unicode Driver}; SERVER=%s; DATABASE=%s; UID=%s; PWD=%s; CHARSET=UTF8", szODBCIP, szODBCName, szODBCID, szODBCPW);
	if (iRetval = SQLDriverConnect(g_hDbc, NULL, szConIn, wcslen(szConIn), NULL, NULL, NULL, SQL_DRIVER_COMPLETE/*SQL_DRIVER_NOPROMPT*/) < 0)
		return iRetval;

	// 4. ��� �ڵ� �Ҵ�
	if (iRetval = SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt) < 0)
		return iRetval;

	return SQL_SUCCESS;
}

void MYSQLDisconnect()
{
	SQLCloseCursor(g_hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	SQLDisconnect(g_hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
}
