#pragma comment(lib,"ws2_32.lib")
#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<stdbool.h>
#define MESSAGESIZE 30 

void error_handling(char* message);

int main(int argc, char* argv[]) {

	SOCKET serv_socket, clnt_socket;
	SOCKADDR_IN serv_addr, clnt_addr;
	WSADATA wsa_data;

	char message[MESSAGESIZE];

	if (argc != 2) {
		printf("Usage: %s <port>  \n", argv[0]);	 /*absolute path argv[0] */
		exit(1);									 /* <stdlib> */
	}
	serv_socket = socket(PF_INET, SOCK_STREAM, 0);

	if (serv_socket == SOCKET_ERROR)
		error_handling("socket() error");
	
	/*initialize*/ 
	memset(&serv_addr, 0, sizeof(serv_addr));		 /* <string.h> */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(atoi(argv[1]));/*atoi -> ASCII to order Int? 
												     + itoa -> Int to order ASCII
			ASCII > INT > 	빅엔디안 32비트 정수값     htonl-> Host to order Network Long*/

	if(bind(serv_socket,(struct sockaddr *)&serv_addr,sizeof(serv_addr)==SOCKET_ERROR))
		error_handling("bind() error!");
	if (listen(serv_socket, 5) == SOCKET_ERROR)      /*listen(SOCKET s,INT backlog)*/
		error_handling("listen() error!");

	int clnt_addr_size = sizeof(clnt_addr);
	clnt_socket = accept(serv_socket, (struct sockaddr*)&clnt_addr, &clnt_addr_size); //????

	if (clnt_socket == INVALID_SOCKET)				 /*INVALID_SOCKET(SOCKET)(~0) e; 유효범위*/
		error_handling("accept() error!");


	int str_len;

	while (true) {

		memset(message, NULL, MESSAGESIZE);						  /*initialize*/
		str_len = recv(clnt_socket, message, MESSAGESIZE - 1, 0); /*recv INT flags? */

		if (str_len == -1)
			error_handling("recv() error!");
		printf("Message fron Client: %s  \n", message);
		puts("Input message:  ");
		gets(message);
		send(clnt_socket, message, strlen(message), 0);
	}
	closesocket(clnt_socket);
	closesocket(serv_socket);
	
	
	return 0;
}
void error_handling(char* message) {

	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
