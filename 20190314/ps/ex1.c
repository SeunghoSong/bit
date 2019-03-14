#include <stdio.h>
#include <stdlib.h>

int main()
	{
	printf("Running ps with system\n");
	system("ps -ax &"); //별도의 프로세서를 생성후 시스템 함수 사용 출력후 부모프로세스로 리턴
	printf("Done.\n");
	return 0;
	// 순차적으로 실행된다.
	
	
	}
