# CPP+DB(MySQL Connector)

  ![capture](https://github.com/kbm0996/-SystemLink-CPPxDB/blob/master/jpg/figure0.png) 

**ODBC**(Open DataBase Connectivity)는 DBMS(Database Management System) 사용을 위한 표준 규격의 API이다. ODBC는 프로그래밍 언어, 데이터베이스 시스템 및 운영 체제와 독립적이다. 
따라서, 사용자는 ODBC 사용법만 따르면 접속처의 DB가 어떠한 DBMS에 관리되고 있는지 어떤 방법으로 사용을 해야하는지 알 필요 없이 접근할 수 있다.

## 📑 ODBC 다운로드 및 설치

[여기](https://dev.mysql.com/downloads/connector/odbc/)에서 MySQL ODBC 드라이버 다운로드



## 📑 사용법

해당 예제는 SQLDriverConnect() 함수를 사용하기 때문에 SQLConnect() 함수와 달리 사용자 DSN 등록하지 않고 드라이버에 문자열로 직접 접근한다. 

자세한 내용은 예제 코드 참고
