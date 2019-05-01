#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include <sql.h>
#include <sqlext.h>
#include <conio.h>
#include <clocale>

/////////////////////////////////////////////
// �ڵ� ���� ����
// ODBC ���� �տ� SQLdl ���� Type ���
/////////////////////////////////////////////
SQLHENV		hEnv;
SQLHDBC		hDbc;
SQLHSTMT	hStmt;

// ������� �ޱ� ���� ����
HRESULT		hResult;

int main()
{
	setlocale(LC_ALL, "");

	WCHAR		State[1024];
	WCHAR		Message[1024];
	SQLSMALLINT	MsgLen;
	int			NativeErr;

	TCHAR		BuffNo[40];
	TCHAR		BuffID[40];
	TCHAR		BuffPass[40];

	SQLLEN		lBuffNo, lBuffID, lBuffPass;

	// Binary
	SQLLEN		Size = SQL_LEN_DATA_AT_EXEC(512);
	SQLPOINTER	Token;

	/////////////////////////////////////////////////
	// ȯ�漳�� �Ҵ� + �����Ӽ� ����
	/////////////////////////////////////////////////
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) return 0;
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS) return 0;

	/////////////////////////////////////////////////
	// �����ڵ� �Ҵ� + ����
	/////////////////////////////////////////////////
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) return 0;
	// �����ڵ鿡 ���� ���� �޽��� ó��
	ZeroMemory(Message, 1024);
	MsgLen = 0;
	SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, 1, (SQLTCHAR*)State, (SQLINTEGER*)&NativeErr, (SQLTCHAR*)Message, 1024, &MsgLen);
	wprintf(L"%s\n", Message);

	// #1 DSN
	///SQLConnect(hDbc, (SQLTCHAR*)"Connect GameSchema", SQL_NTS, (SQLTCHAR*)NULL, SQL_NTS, (SQLTCHAR*)NULL, SQL_NTS);

	// #2
	WCHAR szConnect[1024] = L"Driver={MySQL ODBC 5.3 Unicode Driver}; Server=127.0.0.1; Database=game_schema; User=root; Password=1234; Option=3;";
	SQLTCHAR OutCon[255];
	SQLSMALLINT cbOutCon;

	if (SQLDriverConnect(hDbc, NULL, (SQLTCHAR *)szConnect, wcslen(szConnect), OutCon, sizeof(OutCon), &cbOutCon, SQL_DRIVER_COMPLETE)//SQL_DRIVER_NOPROMPT);
																																	  //SQLDriverConnect(hDbc, NULL, (SQLTCHAR *)szConnect, wcslen(szConnect), OutCon, sizeof(OutCon)/2, &cbOutCon, SQL_DRIVER_NOPROMPT);
																																	  // ���ӿ� ���� �޼��� ó��
		 > 1)
	{
		ZeroMemory(Message, 1024);
		MsgLen = 0;
		SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, 1, (SQLTCHAR*)State, (SQLINTEGER*)&NativeErr, (SQLTCHAR*)Message, 1024, &MsgLen);
		wprintf(L"%s\n", Message);
	}
	/////////////////////////////////////////////////
	// ����ڵ� �Ҵ� + ����
	/////////////////////////////////////////////////
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) return false;

	// ���ε� (select������ ������ ����� �������� ���� + SQL ������ ����)
	SQLBindCol(hStmt, 1, SQL_C_TCHAR, BuffNo, 40, (SQLLEN*)&lBuffNo);
	SQLBindCol(hStmt, 2, SQL_C_TCHAR, BuffID, 40, (SQLLEN*)&lBuffID);
	SQLBindCol(hStmt, 3, SQL_C_TCHAR, BuffPass, 40, (SQLLEN*)&lBuffPass);
	ZeroMemory(Message, 1024);
	MsgLen = 0;
	SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, (SQLTCHAR*)State, (SQLINTEGER*)&NativeErr, (SQLTCHAR*)Message, 1024, &MsgLen);
	wprintf(L"%s\n", Message);
	/*
	// �����ϱ� (INSERT INTO)
	SQLExecDirect(hStmt, (SQLTCHAR*)"INSERT INTO Member(ID,Pass,Name) VALUES('myID', '2222', 'long'", SQL_NTS);
	// ���ӿ� ���� �޼��� ó��
	SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, 1, (SQLTCHAR*)State, (SQLINTEGER*)&NativeErr, (SQLTCHAR*)Message, 1024, &MsgLen);
	wprintf(L"%s\n", Message);
	*/

	// �����ϱ� (SELECT FROM)
	SQLExecDirect(hStmt, (SQLTCHAR*)L"SELECT * FROM account", SQL_NTS);
	ZeroMemory(Message, 1024);
	MsgLen = 0;
	SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, (SQLTCHAR*)State, (SQLINTEGER*)&NativeErr, (SQLTCHAR*)Message, 1024, &MsgLen);
	wprintf(L"%s\n", Message);

	// ��� ���
	while (SQLFetch(hStmt) != SQL_NO_DATA)
	{
		wprintf(L"No: %s, ID:%s, Pass:%s\n", BuffNo, BuffID, BuffPass);
		SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, (SQLTCHAR*)State, (SQLINTEGER*)&NativeErr, (SQLTCHAR*)Message, 1024, &MsgLen);
		//wprintf(L"%s\n", Message);

		/*
		// ���� ������ ����� �ٽ� ������ �������ϴ� ���
		SQLHSTMT hStmt2;
		if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt2) != SQL_SUCCESS) return false;
		hResult = SQLBindCol(hStmt2, 1, SQL_C_CHAR, BuffPass, 8, (SQLLEN*)&lBuffID);
		hResult = SQLExecDirect(hStmt2, (SQLTCHAR*)"SELECT * FROM tblUser", SQL_NTS);
		SQLGetDiagRec(SQL_HANDLE_STMT, hStmt2, 1, (SQLTCHAR*)State, (SQLINTEGER*)&NativeErr, (SQLTCHAR*)Message, 1024, &MsgLen);
		wprintf(L"%s\n", Message);

		while (SQLFetch(hStmt2) != SQL_NO_DATA)
		{
		wprintf(L"ID:%s, Pass:%s, Nick:%s\n", BuffID, BuffPass, BuffNick);
		}
		SQLCloseCursor(hStmt2);
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt2);
		*/
	}

	// ������
	SQLCloseCursor(hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	SQLDisconnect(hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

	/*****************************************************
	* SQL ������ ������ �ѱ۷� �ڼ��� �˷��ִ� �Լ�
	SQLGetDiagRec(
	SQL_HANDLE_ENV,
	DBC,
	STMT,
	�ڵ�, 1, (SQLCHAR*)State,
	(SQLINTEGER*)&NativeErr,
	(SQLCHAR*)Message, // �����޼����� ����Ǵ� ����
	MsgSize, // ���� ������
	&MsgLen) // ���� ����
	* �÷��� ���� �������ִ� �Լ� : Ŭ����ȭ�Ҷ� ���. �׷��� ������ ������ �� ��� X
	SQLNumResultCols(hStmt, (int)&�÷���);
	* �÷��� �̸��� ���´�
	SQLDescribeCol(hStmt, �÷���ȣ, Buffer, sizeof(Buffer), NULL,NULL,NULL,NULL,NULL)
	*****************************************************/

	return 0;
}