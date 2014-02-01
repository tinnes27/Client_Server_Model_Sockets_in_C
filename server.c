/*///////////////////////////////////////////////////////////
*
* FILE:		server.c
* AUTHOR:	Timothy Innes
* PROJECT:	CS 3251 Project 1 - Professor Ellen Zegura 
* DESCRIPTION:	Network Server Code
* CREDIT:	Adapted from Professor Traynor
*
*////////////////////////////////////////////////////////////

/*Included libraries*/

#include <stdio.h>	  /* for printf() and fprintf() */
#include <sys/socket.h>	  /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h>	  /* for sockaddr_in and inet_addr() */
#include <stdlib.h>	  /* supports all sorts of functionality */
#include <unistd.h>	  /* for close() */
#include <string.h>	  /* support any string ops */

#define RCVBUFSIZE 512		/* The receive buffer size */
#define SNDBUFSIZE 512		/* The send buffer size */
#define BUFSIZE 40		/* Your name can be as many as 40 chars*/
#define RECORDSIZE 4

typedef struct bank_account{
    char name[40];
    int balance;
    int count;

}record;



/* helper function that holds bank account information */
void ProcessData(int clientSock, record* records)
{
    int  balance;               /* Place to record account balance result */

    char nameBuf[BUFSIZE];          /* Buff to store account name from client */
    char sndBuf[SNDBUFSIZE];
    memset(&nameBuf, 0, BUFSIZE);
    memset(&sndBuf, 0, SNDBUFSIZE);

    /* delcare an array of structs to hold back account infromation */
    
    int numBytesRcvd;
    if ((numBytesRcvd = recv(clientSock, nameBuf, BUFSIZE, 0)) < 0){
        //printf("recv() failed\n");
        exit(1);
    }
    //printf("test on receive data: %s\n", nameBuf);
    char* code_token = "B";
    if(strncmp(code_token, nameBuf, 1) != 0){
        code_token = "C";
    } 
   // strcpy(coded_token, nameBuf[0] );
    char trimed[sizeof(nameBuf) -1];
    strcpy(trimed, &(nameBuf[1]) );
    
    
    int i;
    int aInt = 1000;
    char numBuff[20];
    int numLength = 0;
    char* code = "B";

    
    for(i=0; i< RECORDSIZE; i++){
        int nameLength = strlen(records[i].name);
        if( strncmp(records[i].name, trimed, nameLength) == 0 ){

            if( strncmp(code_token, code, 1) == 0 ){
                records[i].count = records[i].count + 1;
                aInt = records[i].balance;
            }else{
                aInt = records[i].count;
            }

            sprintf(numBuff, "%d", aInt);
            numLength = strlen(numBuff);
            //printf("we have a match, %d\n", records[i].balance);
        }
    }
    

    while(numBytesRcvd > 0){

        if (send(clientSock, numBuff, numLength, 0) != numLength){
            //printf("send() failed\n");
            //exit(1);
        }

         if ((numBytesRcvd = recv(clientSock, nameBuf, BUFSIZE, 0)) < 0){
            //printf("recv() failed\n");
            //exit(1);
        }

        close(clientSock);

    }

    /* Return account balance to client */
    /*  FILL IN     */
}

/* The main function */
int main(int argc, char *argv[])
{

    int serverSock;				/* Server Socket */
    int clientSock;				/* Client Socket */
    struct sockaddr_in changeServAddr;		/* Local address */
    struct sockaddr_in changeClntAddr;		/* Client address */
    unsigned short changeServPort;		/* Server port */
    unsigned int clntLen;			/* Length of address data struct */

    record records[RECORDSIZE];

    /* initialize mySavings */
    strcpy(records[0].name, "mySavings");
    records[0].balance = 10;
    records[0].count = 0;

    /* initialize myChecking */
    strcpy(records[1].name, "myChecking");
    records[1].balance = 200;
    records[1].count = 0;

    /* initialize myRetirement */
    strcpy(records[2].name, "myRetirement");
    records[2].balance = 300000;
    records[2].count = 0;

    /* initialize myCollege */
    strcpy(records[3].name, "myCollege");
    records[3].balance = 40000;
    records[3].count = 0;



    if(argc != 2){
        printf("You've been served, not! Must include port number\n");
        exit(1);
    }

    /* convert server port number from string to number. */
    changeServPort = atoi(argv[1]); 

    //printf("Server side: test on port number:  %d\n", changeServPort);

    /* Create new TCP Socket for incoming requests*/
    /*      FILL IN */
    if( (serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ){
        printf("server socket creation failure\n");
        exit(1);
    }

    /* Construct local address structure*/
    /*      FILL IN */
    memset(&changeServAddr, 0, sizeof(changeServAddr) );
    changeServAddr.sin_family = AF_INET;
    changeServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    changeServAddr.sin_port = htons(changeServPort);

    /* Bind to local address structure */
    /*      FILL IN */
    if( bind(serverSock, (struct sockaddr*) &changeServAddr, sizeof(changeServAddr)) <0){
        printf("bind failed\n");
        exit(1);
    }

    /* Listen for incoming connections */
    /*      FILL IN */
    if( listen( serverSock, 5) < 0){
        printf("listen failed\n");
        exit(1);
    }
    
    /* Loop server forever*/
    while(1)
    {
        clntLen = sizeof(changeClntAddr);
	/* Accept incoming connection */
	/*	FILL IN	    */
        if( (clientSock = accept(serverSock, (struct sockaddr *) &changeClntAddr, &clntLen) ) < 0){
            printf("Server Side:  client sock failed accept\n");
            exit(1); 
        }

        ProcessData(clientSock, records);

    }

}

/* notes on copying the file to the server
scp foo.sh user@host:~
ssh user@host
./foo.sh
*/

