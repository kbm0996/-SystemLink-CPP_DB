# CPP+DB(MySQL Library)

  ![capture](https://github.com/kbm0996/-SystemLink-CPPxDB/blob/master/jpg/figure0.png) 

MySQL은 JDBC, ODBC, .Net 용 표준 드라이버를 제공하여 개발자가 원하는 언어로 DB 응용 프로그램을 만들 수 있다. 또한 Native C Library를 통해 MySQL을 애플리케이션에 직접 포함시킬 수도 있다.

## 📑 MySQL Library 다운로드 및 사용법

1. [여기](https://dev.mysql.com/downloads/mysql/)에서 사용자 환경에 맞는 버전의 MySQL Server 다운로드. (예제는 Windows 64-bit의 MySQL Community Server 5.7.27 버전 사용)

2. MySQL이 설치된 폴더 내의 include 디렉터리와 lib 디렉터리를 포함시킨다. **1-a** 처럼 프로젝트 내에 복사해도 되고, **1-b** 처럼 MySQL 내에 있는 폴더를 직접 포함시켜도된다.(예제는 편의상 전자의 방법 사용)

  >![capture_manual](https://github.com/kbm0996/-SystemLink-CPPxDB/blob/master/jpg/mysql1a.png) 
  
  **figure 1-a*
  
   >![capture_manual](https://github.com/kbm0996/-SystemLink-CPPxDB/blob/master/jpg/mysql1b1.png) 
   >![capture_manual](https://github.com/kbm0996/-SystemLink-CPPxDB/blob/master/jpg/mysql1b2.png) 
  **figure 1-b*

3. 이하 예제 코드 참고
