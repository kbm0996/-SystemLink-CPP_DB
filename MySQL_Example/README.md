# CPP+DB(MySQL Library)

  ![capture](https://github.com/kbm0996/-SystemLink-CPPxDB/blob/master/jpg/figure0.png) 

**ODBC**(Open DataBase Connectivity)는 DBMS(Database Management System) 사용을 위한 표준 규격의 API이다. ODBC는 프로그래밍 언어, 데이터베이스 시스템 및 운영 체제와 독립적이다. 
따라서, 사용자는 ODBC 사용법만 따르면 접속처의 DB가 어떠한 DBMS에 관리되고 있는지 어떤 방법으로 사용을 해야하는지 알 필요 없이 접근할 수 있다.

## 📑 MySQL Library 다운로드 및 사용법

1. [여기](https://dev.mysql.com/downloads/mysql/)에서 사용자 환경에 맞는 버전의 MySQL Server 다운로드. (예제는 Windows 64-bit의 MySQL Community Server 5.7.27 버전 사용)

2. MySQL이 설치된 폴더 내의 include 디렉터리와 lib 디렉터리를 포함시킨다. **1-a** 처럼 프로젝트 내에 복사해도 되고, **1-b** 처럼 MySQL 내에 있는 폴더를 직접 포함시켜도된다.(예제는 편의상 전자의 방법 사용)

  >![capture_manual](https://github.com/kbm0996/-SystemLink-CPPxDB/blob/master/jpg/mysql1a.png) 
  
  **figure 1-a*
  
   >![capture_manual](https://github.com/kbm0996/-SystemLink-CPPxDB/blob/master/jpg/mysql1b1.png) 
   >![capture_manual](https://github.com/kbm0996/-SystemLink-CPPxDB/blob/master/jpg/mysql1b2.png) 
  **figure 1-b*

3. 이하 예제 코드 참고
