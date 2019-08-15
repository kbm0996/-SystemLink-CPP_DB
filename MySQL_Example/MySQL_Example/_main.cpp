#pragma comment(lib,"mysql/lib/vs14/mysqlclient.lib")
#include "mysql/include/mysql.h"
#include <cstdio>

#define df_DB_IP "127.0.0.1"
#define df_DB_ID "root"
#define df_DB_PASS "1234"
#define df_DB_NAME "game_schema"
#define df_DB_PORT 3306


int main()
{
	MYSQL mysql;
	MYSQL_RES* sqlResultSet;
	MYSQL_ROW sqlRow;
	int query_stat;

	// * mysql_init() : 초기화
	mysql_init(&mysql);

	// * mysql_real_connect() : DB 연결
	if (!mysql_real_connect(&mysql, df_DB_IP, df_DB_ID, df_DB_PASS, df_DB_NAME, df_DB_PORT, NULL, 0))
	{
		// * mysql_errno(), mysql_error() : DB 에러 표시
		// - fprintf() : file printf. 서식있는 출력(printf에 파일 포인터가 인자로 추가)
		// - stdin : 표준 입력 장치(default keyboard)
		// - stdout : 표준 출력 장치(default monitor)
		// - stderr : 표준 에러 출력 장치(default monitor)
		fprintf(stderr, "mysql_real_connect() error : %d %s", mysql_errno(&mysql), mysql_error(&mysql));
		return 1;
	}

	// * mysql_set_character_set() : mysql_real_escape_string()의 캐릭터셋 설정
	//mysql_set_character_set(&mysql, "utf8");

	// * mysql_query() : 쿼리 실행
	// - character_set_connection : 쿼리문에서 인트로듀서가 없는 리터럴 또는 숫자를 문자열로 변환할 때 사용하는 캐릭터셋
	// - character_set_results : MySQL 서버가 쿼리 처리 결과를 클라이언트로 보낼 때 사용하는 캐릭터셋
	// - character_set_client : MySQL 클라이언트의 캐릭터셋
	mysql_query(&mysql,"set session character_set_connection=euckr;");
	mysql_query(&mysql,"set session character_set_results=euckr;");
	mysql_query(&mysql,"set session character_set_client=euckr;");
	if (mysql_query(&mysql, "SELECT * FROM account") != 0)
	{
		fprintf(stderr, "mysql_query() error : %d %s", mysql_errno(&mysql), mysql_error(&mysql));
		return 1;
	}

	// * mysql_store_result(), mysql_use_result() : mysql_query() 실행 성공 시 결과를 출력하기 위해 결과셋에 저장
	// * mysql_fetch_row() : 레코드를 가져오기(fetch). 검색 row가 없거나 에러 발생 시 NULL 리턴
	sqlResultSet = mysql_store_result(&mysql);	
	while ((sqlRow = mysql_fetch_row(sqlResultSet)) != NULL)
	{
		printf("%s %s %s\n", sqlRow[0], sqlRow[1], sqlRow[2]);
	}

	// * mysql_free_result() : 결과셋에 할당된 메모리 해제
	mysql_free_result(sqlResultSet);

	// * mysql_close() : DB 세션 닫기
	mysql_close(&mysql);

	return 0;
}
