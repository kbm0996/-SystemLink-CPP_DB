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
	// DB 연결
	/**********************************************************/
	SQLRETURN iRetval = MYSQLConnect(df_DB_IP, df_DB_NAME, df_DB_ID, df_DB_PASS);
	if (iRetval != 0)
	{
		SQLGetDiagRec(SQL_HANDLE_DBC, g_hDbc, 1, (SQLTCHAR*)szStat, (SQLINTEGER*)&iNativeErr, (SQLTCHAR*)szMsg, 1024, &iMsglen);
		wprintf(L"%s\n", szMsg);
		return iRetval;
	}
	
	/**********************************************************/
	// 쿼리 실행
	/**********************************************************/
	SQLCHAR		BuffNo[40], BuffID[40], BuffPass[1000];
	SQLLEN		lBuffNo = 0, lBuffID = 0, lBuffPass = 0;
	// 바인딩 (select문에서 데이터 목록을 가져오기 위함 + SQL 인젝션 방지)
	// * SQLBindCol(명령핸들, 컬럼번호, 해당컬럼의 데이터유형, 버퍼, 버퍼 '길이', )  
	SQLBindCol(g_hStmt, 1, SQL_C_TCHAR, BuffNo, sizeof(BuffNo), (SQLLEN*)&lBuffNo);
	SQLBindCol(g_hStmt, 2, SQL_C_TCHAR, BuffID, sizeof(BuffID), (SQLLEN*)&lBuffID);
	SQLBindCol(g_hStmt, 3, SQL_C_TCHAR, BuffPass, sizeof(BuffPass), (SQLLEN*)&lBuffPass);

	///SQLExecDirect(hStmt, (SQLTCHAR*)"INSERT INTO Member(ID,Pass,Name) VALUES('myID', '2222', 'long'", SQL_NTS);
	SQLExecDirect(g_hStmt, (SQLTCHAR*)L"SELECT * FROM account", SQL_NTS);;

	/**********************************************************/
	// 결과 출력
	/**********************************************************/
	// * 컬럼의 수를 리턴해주는 함수 : 클래스화할때 사용. 성능은 떨어짐
	//	SQLNumResultCols(hStmt, (int)&컬럼값);
	// * 컬럼 이름 얻어오기
	//	SQLDescribeCol(hStmt, 컬럼번호, Buffer, sizeof(Buffer), NULL,NULL,NULL,NULL,NULL)
	while (SQLFetch(g_hStmt) != SQL_NO_DATA)
	{
		wprintf(L"%s	%s	%s\n", BuffNo, BuffID, BuffPass);
		SQLGetDiagRec(SQL_HANDLE_STMT, g_hStmt, 1, (SQLTCHAR*)szStat, (SQLINTEGER*)&iNativeErr, (SQLTCHAR*)szMsg, 1024, &iMsglen);
		//wprintf(L"%s\n", szMsg);

		/*
		// 쿼리 내에서 결과로 다시 쿼리를 날려야하는 경우
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
	// DB 연결 해제
	/**********************************************************/
	MYSQLDisconnect();

	return 0;
}

SQLRETURN MYSQLConnect(const WCHAR * szODBCIP, const WCHAR * szODBCName, const WCHAR * szODBCID, const WCHAR * szODBCPW)
{
	SQLRETURN iRetval = SQL_SUCCESS;

	////////////////////////////////////////////////////////////////
	// 핸들 할당
	//
	////////////////////////////////////////////////////////////////
	// 1. 환경 핸들 할당
	// * SQLAllocHandle() : 환경, 연결, 명령문 핸들 할당
	if (iRetval = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &g_hEnv) < 0) // 0 = SQL_SUCCESS, 1 = SQL_SUCCESS_WITH_INFO
		return iRetval;

	// 2. ODBC 드라이버 버전 설정
	// * SQLSetEnvAttr() : 환결 제어 특성 설정
	if (iRetval = SQLSetEnvAttr(g_hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0) < 0)
		return iRetval;

	// 3. 연결 핸들 할당
	if (iRetval = SQLAllocHandle(SQL_HANDLE_DBC, g_hEnv, &g_hDbc) < 0)
		return iRetval;


	////////////////////////////////////////////////////////////////
	// DBMS 연결
	//
	////////////////////////////////////////////////////////////////
	// 3. DB 연결 방법 : DSN (only x32)
	// * SQLConnect() : 연결 핸들로 드라이버 및 데이터 원본에 대한 연결 설정
	// - ODBC 드라이버 DSN 설정 : 제어판 ~ 관리도구 ~ 데이터원본(ODBC) ~ 사용자 DSN 탭
	//if (iRetval = SQLConnect(g_hDbc, (SQLTCHAR*)"connect_gameschema", SQL_NTS, (SQLTCHAR*)szODBCID, SQL_NTS, (SQLTCHAR*)szODBCPW, SQL_NTS) < 0)
	//	return iRetval;
	
	// 3. DB 연결 방법 : Driver
	// * SQLDriverConnect() : 핸들없이 각종 정보를 포함하는 문자열을 사용하여 연결 설정
	// - ODBC 드라이버명 조회 : 제어판 ~ 관리도구 ~ 데이터원본(ODBC) ~ 드라이버탭
	WCHAR szConIn[255] = L"\0";
	swprintf_s(szConIn, L"Driver={MySQL ODBC 5.3 Unicode Driver}; Server=%s; Database=%s; User=%s; Password=%s", szODBCIP, szODBCName, szODBCID, szODBCPW);
	if (iRetval = SQLDriverConnect(g_hDbc, NULL, szConIn, wcslen(szConIn), NULL, NULL, NULL, SQL_DRIVER_COMPLETE/*SQL_DRIVER_NOPROMPT*/) < 0)
		return iRetval;

	// 4. 명령 핸들 할당
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
