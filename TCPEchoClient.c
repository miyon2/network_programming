#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE      32

#define EchoReq 	"EchoReq|"
#define FileUpReq	"FileUpReq|"
#define FileGetReq	"FileGetReq|"
#define FileListReq	"FileListReq"
#define EchoRep		"EchoRep|"
#define FileAck		"FileAck|"
#define Delimeter	"|"

void DieWithError(char* errorMessage);

int main(int argc, char *argv[]){
	int sock;
	int first_flag = 0;				// 0 is first, 1 is after
	struct sockaddr_in echoServAddr;
	unsigned short echoServPort;
	char *servIP = malloc(sizeof(char) * 16);
	char *echoString = malloc(sizeof(char) * 256);
	char *tmpPort = malloc(sizeof(char) * 6);
	char *first_str = malloc(sizeof(char) * 20);
	char echoBuffer[RCVBUFSIZE];
	unsigned int firstStringLen;
	unsigned int echoStringLen;
	int bytesRcvd, totalBytesRcvd;

	//variables for FT
	char msgType[20];
	char *ftString;
	char *ftFileName = malloc(sizeof(char) * 50);

	printf("server ip : ");
	scanf("%s", servIP);
	printf("port : ");
	scanf("%s", tmpPort);

	echoServPort = atoi(tmpPort);
	/*
	if((argc < 3) || (argc > 4)){
		fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
		exit(1);
	}

	servIP = argv[1];
	echoString = argv[2];

	if(argc == 4){
		echoServPort = atoi(argv[3]);
	}
	else{
		echoServPort = 7;
	}
	*/

	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		DieWithError("socket() failed");
	}

	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family 	= AF_INET;
	echoServAddr.sin_addr.s_addr 	= inet_addr(servIP);
	echoServAddr.sin_port 		= htons(echoServPort);

	if(connect(sock, (struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) < 0){
		DieWithError("connect() failed");
	}

	while(strcmp(echoString, "quit") != 0){		
		if(first_flag != 0){
			printf("msg-> ");
			scanf("%s", echoString);
			echoStringLen = strlen(echoString);

			if(send(sock, echoString, echoStringLen, 0) != echoStringLen){
				DieWithError("send() sent a diffrent number of bytes than expected");
			}
			
			// client input strings, TF
			if(strcmp(echoString, "TF") == 1){
				printf("Welcome to Socket FT client!\n");
				printf("ftp command [p)ut   g)et   l)s   r)ls   e)xit]->");
				scanf("%c", ftString);
	
				/*
				#define FileUpReq       11
				#define FileListReq     12
				#define FileAck         13
				#define FTEnd           14
				*/
	
				while((*ftString) != 'e'){
					switch(*ftString){
						case 'p':
							strcpy(msgType,FileUpReq);
							// send put request to server
							if(send(sock, echoString, strlen(echoString), 0) != 1){
								DieWithError("send() sent a diffrent number of bytes than expected");
							}
	
							// receiving fileack from server
							if(bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0) <= 0 && echoBuffer[0] == 'a'){
								DieWithError("recv() failed or connection closed premturely");
							}
	
							printf("filename to put to server ->");
							scanf("%s", echoString);
	
							if(send(sock, echoString, echoStringLen, 0) != echoStringLen){
								DieWithError("send() sent a diffrent number of bytes than expected");
							}
						
							/*
							// Receiving file name or get file						
							totalBytesRcvd = 0;
		
							while(totalBytesRcvd < echoStringLen){
								if((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0){
									DieWithError("recv() failed or connection closed premturely");
								}
		
								totalBytesRcvd += bytesRcvd;
								echoBuffer[bytesRcvd] = '\0';
								printf("%s\n", echoBuffer);
							}
							*/
							break;
						case 'g':
							strcpy(msgType, FileGetReq);
							// send put request to server
							if(send(sock, msgType, strlen(msgType), 0) != 2){
								DieWithError("send() sent a diffrent number of bytes than expected");
							}
	
							// receiving fileack from server
							if(bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0) <= 0 && echoBuffer[0] == 'a'){
								DieWithError("recv() failed or connection closed premturely");
							}

							printf("filename to put to server ->");
							scanf("%s", echoString);	

							if(send(sock, echoString, echoStringLen, 0) != echoStringLen){
								DieWithError("send() sent a diffrent number of bytes than expected");
							}
							break;
						case 'l':
							break;
						case 'r':
							strcpy(msgType, FileListReq);
							// send file list request to server
							if(send(sock, msgType, strlen(msgType), 0) != 1){
								DieWithError("send() sent a diffrent number of bytes than expected");
							}
	
							// receiving fileack from server
							if(bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0) <= 0 && echoBuffer[0] == 'a'){
								DieWithError("recv() failed or connection closed premturely");
							}
	
							printf("list request to server ->");
							scanf("%s", echoString);
	
							if(send(sock, echoString, echoStringLen, 0) != echoStringLen){
								DieWithError("send() sent a diffrent number of bytes than expected");
							}
							break;
						case 'e':
							break;
					}
					printf("ftp command [p)ut   g)et   l)s   r)ls   e)xit]->");
					scanf("%c", ftString);
				}
			}
			else{
				totalBytesRcvd = 0;
				printf("msg<- ");

				while(totalBytesRcvd < echoStringLen){
					if((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0){
						DieWithError("recv() failed or connection closed premturely");
					}
					totalBytesRcvd += bytesRcvd;
					echoBuffer[bytesRcvd] = '\0';
					printf("%s\n", echoBuffer);
				}
			}
		}
		else{
			first_str = "Hello via client";
			printf("msg-> %s\n", first_str);
			echoStringLen = strlen(first_str);

			if(send(sock, first_str, echoStringLen, 0) != echoStringLen){
				DieWithError("send() sent a diffrent number of bytes than expected");
			}

			first_flag = 1;

			totalBytesRcvd = 0;

			printf("msg<- ");

			if((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0){
				DieWithError("recv() failed or connection closed premturely");
			}

			totalBytesRcvd += bytesRcvd;
			echoBuffer[bytesRcvd] = '\0';
			printf("%s\n", echoBuffer);
		}
	}
	close(sock);
	free(echoString);
	free(servIP);
	free(tmpPort);
	exit(0);
}
