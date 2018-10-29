#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#define RCVBUFSIZE 32

void DieWithError(char *errorMessage);

void HandleTCPClient(int clntSocket){
	char echoBuffer[RCVBUFSIZE];
	int recvMsgSize;
	int first_flag = 0;
	char *first_str = malloc(sizeof(char) * 20);
	first_str = "Hi via server";

	//making log file
	FILE *log;
	log = fopen("echo_history.log", "a+");

	// flush weird string that already stored in buffer
	memset(echoBuffer, 0, RCVBUFSIZE);

	if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0){
		DieWithError("recv() failed");
	}

	printf("msg<- %s\n", echoBuffer);

	while(recvMsgSize > 0){
		if(first_flag != 0){
			if(send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize){
				DieWithError("send() failed");
			}
	
			printf("msg-> %s\n", echoBuffer);

			fprintf(log, "%s\n", echoBuffer);
			memset(echoBuffer, 0, RCVBUFSIZE);
	
			if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0){
				DieWithError("recv() failed");
			}
		
			printf("msg<- %s\n", echoBuffer);
		}
		else{
			if(send(clntSocket, first_str, strlen(first_str), 0) != strlen(first_str)){
				DieWithError("send() failed");
			}

			printf("msg-> %s\n", first_str);

			fprintf(log, "%s\n", first_str);

			first_flag = 1;
			memset(echoBuffer, 0, RCVBUFSIZE);

			if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0){
				DieWithError("recv() failed");
			}

			printf("msg<- %s\n", echoBuffer);
		}
	}

	// flush the echoBuffer
	memset(echoBuffer, 0, RCVBUFSIZE);

	close(clntSocket);

	fclose(log);
}
