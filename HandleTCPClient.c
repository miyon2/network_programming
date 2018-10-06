#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#define RCVBUFSIZE 32

void DieWithError(char *errorMessage);

void HandleTCPClient(int clntSocket){
	char echoBuffer[RCVBUFSIZE];
	int recvMsgSize;

	//making log file
	FILE *log;
	log = fopen("echo_history.log", "a+");

	// flush weird string that already stored in buffer
	memset(echoBuffer, 0, RCVBUFSIZE);

	if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0){
		DieWithError("recv() failed");
	}

	while(recvMsgSize > 0){
		if(send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize){
			DieWithError("send() failed");
		}

		printf("%s\n", echoBuffer);
		fprintf(log, "%s\n", echoBuffer);

		if((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0){
			DieWithError("recv() failed");
		}
	}

	// flush the echoBuffer
	memset(echoBuffer, 0, RCVBUFSIZE);

	close(clntSocket);

	fclose(log);
}
