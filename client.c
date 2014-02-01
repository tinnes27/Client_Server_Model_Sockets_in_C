/*///////////////////////////////////////////////////////////
*
* FILE:		client.c
* AUTHOR:	Timothy Innes
* PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura 
* DESCRIPTION:	Network Client Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/* Included libraries */

#include <stdio.h>		    /* for printf() and fprintf() */
#include <sys/socket.h>		    /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>		    /* for sockaddr_in and inet_addr() */
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/* Constants */
#define RCVBUFSIZE 512		    /* The receive buffer size */
#define SNDBUFSIZE 512		    /* The send buffer size */
#define REPLYLEN 32

/* The main function */
int main(int argc, char *argv[])
{

    int clientSock;		    /* socket descriptor */
    struct sockaddr_in serv_addr;   /* server address structure */

    char *accountName;		    /* Account Name  */
    char *servIP;		    /* Server IP address  */
    unsigned short servPort;	    /* Server Port number */
    char *marker; /* for Count or Bal term */
    

    char sndBuf[SNDBUFSIZE];	    /* Send Buffer */
    char rcvBuf[RCVBUFSIZE];	    /* Receive Buffer */
    
    int balance;		    /* Account balance */
    char *bal = "BAL";
    char *count = "COUNT";

    /* Get the Account Name from the command line */
    if (argc != 5) 
    {
	printf("Incorrect number of arguments. The correct format is:\n\taccountName serverIP serverPort BAL/COUNT\n");
	exit(1);
    }
    accountName = argv[1];

    memset(&sndBuf, 0, SNDBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    /* Get the addditional parameters from the command line */
    /*	    FILL IN	*/
    servIP = argv[2];
    servPort = atoi(argv[3]);
    marker = argv[4];

    if( (strncmp(marker, bal, 3) != 0) && (strncmp(marker, count, 5) != 0) ){
      printf("Ivalide system request:  Must use BAL or COUNT, instead of %s\n", marker);
      exit(1);
    }
    
    char* tok_B = "B";
    char* tok_C = "C";
   
    if( strncmp(marker, bal, 3) == 0 ){
        strcpy(sndBuf, tok_B);
        strcat(sndBuf, accountName);
    }else{
        strcpy(sndBuf, tok_C);
        strcat(sndBuf, accountName);
    }

   // strcpy(sndBuf, accountName);  //printf("Test on send buffer:  %s\n", sndBuf);
    //printf("test on coded send:  %s\n", sndBuf);

    /* Create a new TCP socket*/
    /*	    FILL IN	*/

    clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(clientSock < 0){
        printf("socked creation failure in client\n");
        exit(1);
    }
    //printf("Client Side:  sock #%d\n", clientSock);

    /* Construct the server address structure */
    /*	    FILL IN	 */
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    int rtnVal = inet_pton(AF_INET, servIP, &serv_addr.sin_addr.s_addr);
    if(rtnVal <= 0){
        printf("IP address conversion from dotted quad to binary failed\n");
        exit(1);
    }

    serv_addr.sin_port = htons(servPort); 

   // printf("Client Side:  server ret #%d\n", rtnVal);
    /* Establish connecction to the server */
    /*	    FILL IN	 */
    if(connect(clientSock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        printf("Client connection faild\n");
        exit(1);
    }
    
    /* Send the string to the server */
    /*	    FILL IN	 */

    //size_t studentName_length = strlen(accountName);

    ssize_t numBytes = send(clientSock, sndBuf, SNDBUFSIZE, 0);

    if(numBytes < 0)
    {
        printf("Send failure\n");
        //exit(1);
    }
    else if(numBytes != SNDBUFSIZE)
    {
        printf("Send incomplete\n");
        //exit(1);
    }

    //printf("Client Side:  bytes sent #%d\n", numBytes);
    /* Receive and print response from the server */
    /*	    FILL IN	 */
    unsigned int totalBytesRcvd = 0;

    while( numBytes = recv(clientSock, rcvBuf, RCVBUFSIZE-1, 0) > 0 ){
        
        
        totalBytesRcvd += numBytes;
        //rcvBuf[numBytes] = '\0';
        //fputs(rcvBuf, stdout);
    }
    printf("%s\n", accountName);
    if( strncmp(marker, bal, 3) == 0 ){
        fputs("Balance is:  ", stdout);  
    }else{
        fputs("Count is:  ", stdout); 
    }
    printf("  %s\n", rcvBuf);

    
    return 0;
}

/*

./nameChanger MyName 69.180.27.210 6000

ssh tinnes3@shuttle5.cc.gatech.edu

scp changeServer tinnes3@shuttle5.cc.gatech.edu:~

./changeServer 6000


*/

