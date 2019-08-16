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

	// * mysql_init() : �ʱ�ȭ
	mysql_init(&mysql);

	// * mysql_real_connect() : DB ����
	if (!mysql_real_connect(&mysql, df_DB_IP, df_DB_ID, df_DB_PASS, df_DB_NAME, df_DB_PORT, NULL, 0))
	{
		// * mysql_errno(), mysql_error() : DB ���� ǥ��
		// - fprintf() : file printf. �����ִ� ���(printf�� ���� �����Ͱ� ���ڷ� �߰�)
		// - stdin : ǥ�� �Է� ��ġ(default keyboard)
		// - stdout : ǥ�� ��� ��ġ(default monitor)
		// - stderr : ǥ�� ���� ��� ��ġ(default monitor)
		fprintf(stderr, "mysql_real_connect() error : %d %s", mysql_errno(&mysql), mysql_error(&mysql));
		return 1;
	}

	// * mysql_set_character_set() : mysql_real_escape_string()�� ĳ���ͼ� ����
	//mysql_set_character_set(&mysql, "utf8");

	// * mysql_query() : ���� ����
	// - character_set_connection : ���������� ��Ʈ�ε༭�� ���� ���ͷ� �Ǵ� ���ڸ� ���ڿ��� ��ȯ�� �� ����ϴ� ĳ���ͼ�
	// - character_set_results : MySQL ������ ���� ó�� ����� Ŭ���̾�Ʈ�� ���� �� ����ϴ� ĳ���ͼ�
	// - character_set_client : MySQL Ŭ���̾�Ʈ�� ĳ���ͼ�
	mysql_query(&mysql,"set session character_set_connection=euckr;");
	mysql_query(&mysql,"set session character_set_results=euckr;");
	mysql_query(&mysql,"set session character_set_client=euckr;");
	if (mysql_query(&mysql, "SELECT * FROM account") != 0)
	{
		fprintf(stderr, "mysql_query() error : %d %s", mysql_errno(&mysql), mysql_error(&mysql));
		return 1;
	}

	// * mysql_store_result(), mysql_use_result() : mysql_query() ���� ���� �� ����� ����ϱ� ���� ����¿� ����
	// * mysql_fetch_row() : ���ڵ带 ��������(fetch). �˻� row�� ���ų� ���� �߻� �� NULL ����
	sqlResultSet = mysql_store_result(&mysql);	
	while ((sqlRow = mysql_fetch_row(sqlResultSet)) != NULL)
	{
		printf("%s	%s	%s\n", sqlRow[0], sqlRow[1], sqlRow[2]);
	}

	// * mysql_free_result() : ����¿� �Ҵ�� �޸� ����
	mysql_free_result(sqlResultSet);

	// * mysql_close() : DB ���� �ݱ�
	mysql_close(&mysql);

	return 0;
}
