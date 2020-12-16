#include <stdio.h>	
#include <stdlib.h>	
#include <string.h>	
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_LEN 1024


int main(int argc, char *argv[])
{
	int soc;
	char message[BUF_LEN];
	int str_len;
	struct sockaddr_in server_adr;

	if(argc!=3)
	{
		printf("다음 형식으로 입력하시오. : %s <IP> <port>\n", argv[0]);
		exit(0);
	}
	
	soc = socket(PF_INET, SOCK_STREAM, 0);
	
	if(soc == -1)
	{
		printf("소켓 생성에 실패하였습니다.\n"); 
		exit(0);
	}	
	
	memset(&server_adr, 0, sizeof(server_adr)); 
	server_adr.sin_family = AF_INET;
	server_adr.sin_addr.s_addr = inet_addr(argv[1]); 
	server_adr.sin_port = htons(atoi(argv[2]));

	if(connect(soc, (struct sockaddr*)&server_adr, sizeof(server_adr)) == -1) 
	{
		printf("connect() 동작에 실패하였습니다. 연결할 수 없습니다.\n");
		exit(0);
	}
	else
		printf("연결되었습니다.\n"); 
	
	while(1)
	{
		fputs("메시지를 입력하시오. : ", stdout); 
		fgets(message, BUF_LEN, stdin);
	
		write(soc, message, strlen(message)); 
		str_len = read(soc, message, BUF_LEN-1); 
		message[str_len]='\0'; 
		
		printf("서버로부터 온 메시지입니다. : %s", message);
		
		break;
	}
	
	close(soc);	// 소켓 close
	
	return 0;
}

