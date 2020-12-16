#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_LEN 1024

int main(int argc, char *argv[])
{
	char message[BUF_LEN];
	int server_fd, client_fd; 
	int msg_len;
	struct sockaddr_in server_addr; 
	struct sockaddr_in client_addr; 
	char ip[30]; 
	

	if(argc < 3)
	{	
		printf("다음 형식으로 입력하시오. : %s <port> [-e[-b]]\n", argv[0]);	
		exit(0);	
	}
	
	server_fd = socket(PF_INET, SOCK_STREAM, 0); 
	
	if (server_fd == -1) 
	{
		printf("소켓 생성에 실패하였습니다.\n");
		exit(0);
	}
			
	memset(&server_addr, 0, sizeof(server_addr)); 
	
	server_addr.sin_family = AF_INET; 
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);	
	server_addr.sin_port = htons(atoi(argv[1])); 
	
	if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		printf("bind() 동작에 실패하였습니다.\n"); 
		exit(0);
	}
	
	if(listen(server_fd, 5) == -1) 
	{
		printf("listen() 동작에 실패하였습니다.\n"); 
		exit(0);
	}
	
	memset(message, 0x00, sizeof(message)); 
	
	printf("클라이언트의 연결을 기다리고 있습니다.\n");
	
	int size =  sizeof(client_addr);
	

	while(1)
	{
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &size);
		
		if(client_fd == -1) 
		{
			printf("accept() 동작에 실패하였습니다.\n"); 
			exit(0);
		}
		
		inet_ntop(PF_INET, &client_addr.sin_addr.s_addr, ip, sizeof(ip)); 
		
		printf("%s 로 클라이언트와 연결되었습니다.\n", ip); 
		
		msg_len = read(client_fd, message, 1024);
		
		write(client_fd, message, msg_len); 
		
		close(client_fd);
		
		printf("%s 연결 소켓을 닫습니다.\n", ip); 
		
	}


	close(server_fd); 
	
	return 0;
}