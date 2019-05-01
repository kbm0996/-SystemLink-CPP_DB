#pragma comment(lib,"mysql/lib/vs14/mysqlclient.lib")
#include "mysql/include/mysql.h"

int main()
{
	MYSQL conn;
	MYSQL *connection = NULL;
	MYSQL_RES *sql_result;
	MYSQL_ROW sql_row;
	int query_stat;

	// 초기화
	mysql_init(&conn);

	// DB 연결
	connection = mysql_real_connect(&conn, "127.0.0.1", "root", "1234", "game_schema", 3306, (char *)NULL, 0);
	if (connection == NULL)
	{
		fprintf(stderr, "Mysql connection error : %s", mysql_error(&conn));
		return 1;
	}

	//한글사용을위해추가.
	mysql_set_character_set(connection, "utf8");


	//	mysql_query(connection,"set session character_set_connection=euckr;");
	//	mysql_query(connection,"set session character_set_results=euckr;");
	//	mysql_query(connection,"set session character_set_client=euckr;");


	// Select 쿼리문
	char *query = "SELECT * FROM account";	// From 다음 DB에 존재하는 테이블 명으로 수정하세요
	query_stat = mysql_query(connection, query);
	if (query_stat != 0)
	{
		printf("Mysql query error : %s", mysql_error(&conn));
		return 1;
	}

	// 결과출력
	sql_result = mysql_store_result(connection);		// 결과 전체를 미리 가져옴
														//	sql_result=mysql_use_result(connection);		// fetch_row 호출시 1개씩 가져옴

	while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
	{
		printf("%2s %2s %s\n", sql_row[0], sql_row[1], sql_row[2]);
	}
	mysql_free_result(sql_result);

	// DB 연결닫기
	mysql_close(connection);

	return 0;
}
