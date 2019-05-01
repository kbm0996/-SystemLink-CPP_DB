#include <Windows.h>
#include <cstdio>
#include <cstdlib>
#include <sql.h>
#include <sqlext.h>
#include <conio.h>
#include <clocale>

/////////////////////////////////////////////
// 핸들 변수 선언
// ODBC 사용시 앞에 SQLdl 붙은 Type 사용
/////////////////////////////////////////////
SQLHENV		hEnv;
SQLHDBC		hDbc;
SQLHSTMT	hStmt;

// 결과값을 받기 위한 변수
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
	// 환경설정 할당 + 버전속성 설정
	/////////////////////////////////////////////////
	if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS) return 0;
	if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS) return 0;

	/////////////////////////////////////////////////
	// 접속핸들 할당 + 연결
	/////////////////////////////////////////////////
	if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS) return 0;
	// 접속핸들에 대한 에러 메시지 처리
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
																																	  // 접속에 대한 메세지 처리
		 > 1)
	{
		ZeroMemory(Message, 1024);
		MsgLen = 0;
		SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, 1, (SQLTCHAR*)State, (SQLINTEGER*)&NativeErr, (SQLTCHAR*)Message, 1024, &MsgLen);
		wprintf(L"%s\n", Message);
	}
	/////////////////////////////////////////////////
	// 명령핸들 할당 + 실행
	/////////////////////////////////////////////////
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS) return false;

	// 바인딩 (select문에서 데이터 목록을 가져오기 위함 + SQL 인젝션 방지)
	SQLBindCol(hStmt, 1, SQL_C_TCHAR, BuffNo, 40, (SQLLEN*)&lBuffNo);
	SQLBindCol(hStmt, 2, SQL_C_TCHAR, BuffID, 40, (SQLLEN*)&lBuffID);
	SQLBindCol(hStmt, 3, SQL_C_TCHAR, BuffPass, 40, (SQLLEN*)&lBuffPass);
	ZeroMemory(Message, 1024);
	MsgLen = 0;
	SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, (SQLTCHAR*)State, (SQLINTEGER*)&NativeErr, (SQLTCHAR*)Message, 1024, &MsgLen);
	wprintf(L"%s\n", Message);
	/*
	// 실행하기 (INSERT INTO)
	SQLExecDirect(hStmt, (SQLTCHAR*)"INSERT INTO Member(ID,Pass,Name) VALUES('myID', '2222', 'long'", SQL_NTS);
	// 접속에 대한 메세지 처리
	SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, 1, (SQLTCHAR*)State, (SQLINTEGER*)&NativeErr, (SQLTCHAR*)Message, 1024, &MsgLen);
	wprintf(L"%s\n", Message);
	*/

	// 실행하기 (SELECT FROM)
	SQLExecDirect(hStmt, (SQLTCHAR*)L"SELECT * FROM account", SQL_NTS);
	ZeroMemory(Message, 1024);
	MsgLen = 0;
	SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, (SQLTCHAR*)State, (SQLINTEGER*)&NativeErr, (SQLTCHAR*)Message, 1024, &MsgLen);
	wprintf(L"%s\n", Message);

	// 결과 출력
	while (SQLFetch(hStmt) != SQL_NO_DATA)
	{
		wprintf(L"No: %s, ID:%s, Pass:%s\n", BuffNo, BuffID, BuffPass);
		SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, (SQLTCHAR*)State, (SQLINTEGER*)&NativeErr, (SQLTCHAR*)Message, 1024, &MsgLen);
		//wprintf(L"%s\n", Message);

		/*
		// 쿼리 내에서 결과로 다시 쿼리를 날려야하는 경우
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

	// 마무리
	SQLCloseCursor(hStmt);
	SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	SQLDisconnect(hDbc);
	SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
	SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

	/*****************************************************
	* SQL 에러의 내용을 한글로 자세히 알려주는 함수
	SQLGetDiagRec(
	SQL_HANDLE_ENV,
	DBC,
	STMT,
	핸들, 1, (SQLCHAR*)State,
	(SQLINTEGER*)&NativeErr,
	(SQLCHAR*)Message, // 에러메세지가 저장되는 버퍼
	MsgSize, // 버퍼 사이즈
	&MsgLen) // 버퍼 길이
	* 컬럼의 수를 리턴해주는 함수 : 클래스화할때 사용. 그러나 성능이 떨어져 잘 사용 X
	SQLNumResultCols(hStmt, (int)&컬럼값);
	* 컬럼의 이름을 얻어온다
	SQLDescribeCol(hStmt, 컬럼번호, Buffer, sizeof(Buffer), NULL,NULL,NULL,NULL,NULL)
	*****************************************************/

	return 0;
}