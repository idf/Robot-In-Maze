#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "rs232.h"
#define GRID_SIZE 10
#define UNO_PORT 24
#define MYBAUD 115200
#define BUF_SIZE 64

//Thread Message Passing
//PI <-> Arduino
char ardPi[BUF_SIZE];
char piArd[BUF_SIZE];
bool ardPiData = false;
bool piArdData = false;
pthread_mutex_t ardPiMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t piArdMutex = PTHREAD_MUTEX_INITIALIZER;

//PI <-> Bt
char btPi[BUF_SIZE];
char piBt[BUF_SIZE];
bool btPiData = false;
bool piBtData = false;
pthread_mutex_t btPiMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t piBtMutex = PTHREAD_MUTEX_INITIALIZER;


//Threading vars
int retArd=0;
pthread_t threadArd;
int retBt=0;
pthread_t threadBt;
pthread_t doBtThreadWrite;
pthread_t doBtThreadRead;
int btThreadCountR = 1;
int btThreadCountW = 1;

void unoSendPi(char **msg)
{
	pthread_mutex_lock(&ardPiMutex);
	memset(&ardPi,0,BUF_SIZE);
	memcpy(&ardPi,msg,BUF_SIZE);
	ardPiData = true;
	pthread_mutex_unlock(&ardPiMutex);
}

void piSendUno(char **msg)
{
	pthread_mutex_lock(&piArdMutex);
	memset(&piArd,0,BUF_SIZE);
	memcpy(&piArd,msg,BUF_SIZE);
	piArdData = true;
	pthread_mutex_unlock(&piArdMutex);
}

void getArdPiMsg(char **msg)
{
	pthread_mutex_lock(&ardPiMutex);
	//Only clear buffer if new message
	memset(msg,0,BUF_SIZE);
	memcpy(msg,&ardPi,BUF_SIZE);
	//Done with data clear shared buffer
	memset(&ardPi,0,BUF_SIZE);
	ardPiData = false;
	pthread_mutex_unlock(&ardPiMutex);
}

void getPiArdMsg(char **msg)
{
	pthread_mutex_lock(&piArdMutex);
	memcpy(msg,&piArd,BUF_SIZE);
	//Done with data, clear shared buffer
	memset(&piArd,0,BUF_SIZE);
	piArdData = false;
	pthread_mutex_unlock(&piArdMutex);
}

void btSendPi(char **msg)
{
	pthread_mutex_lock(&btPiMutex);
	memset(&btPi,0,BUF_SIZE);
	memcpy(&btPi,msg,BUF_SIZE);
	btPiData = true;
	pthread_mutex_unlock(&btPiMutex);
}

void piSendBt(char **msg)
{
	pthread_mutex_lock(&piBtMutex);
	memset(&piBt,0,BUF_SIZE);
	memcpy(&piBt,msg,BUF_SIZE);
	piBtData = true;
	pthread_mutex_unlock(&piBtMutex);
}

void getBtPiMsg(char **msg)
{
	pthread_mutex_lock(&btPiMutex);
	//Only clear buffer if new message
	memset(msg,0,BUF_SIZE);
	memcpy(msg,&btPi,BUF_SIZE);
	//Done with data clear shared buffer
	memset(&btPi,0,BUF_SIZE);
	btPiData = false;
	pthread_mutex_unlock(&btPiMutex);
}

void getPiBtMsg(char **msg)
{
	pthread_mutex_lock(&piBtMutex);
	memcpy(msg,&piBt,BUF_SIZE);
	//Done with data, clear shared buffer
	memset(&piBt,0,BUF_SIZE);
	piBtData = false;
	pthread_mutex_unlock(&piBtMutex);
}

void threadfuncArd()
{
	int n=0;
	char buffer[BUF_SIZE];
	char msgbuf[BUF_SIZE];
	//Open port
	RS232_OpenComport(UNO_PORT,MYBAUD);
	usleep(1000*1000);
	while(1)
	{
		memset(&buffer,0,BUF_SIZE);
		n = RS232_PollComport(UNO_PORT, buffer, BUF_SIZE-1);
		if(n > 0)
		{
			buffer[n] = 0;
			//Incoming data on serial
			if(!ardPiData)
			{
				unoSendPi(&buffer);
			}
		}
		else
		{
			//printf("piArdData: %s\n",(piArdData)?"true":"false");
			//Incoming data on messagebox from PI
			if(piArdData)
			{
				memset(&msgbuf,0,BUF_SIZE);
				getPiArdMsg(&msgbuf);
			}
			else
			{
				RS232_cputs(UNO_PORT,msgbuf);
				memset(&msgbuf,0,BUF_SIZE);
			}
		}
		//According to library developers, 100ms sleep is needed here
		usleep(100*1000);
	}
}

void *doThreadBtWrite(void *pvsock)
{
	printf("BT Write_%d: Spawned\n",btThreadCountW);
        btThreadCountW++;
	int bytes_read=0;
	char buf[BUF_SIZE] = { 0 };
	int *client = (int*)pvsock;
	while(1)
        {
                        if(piBtData)
                        {
                                char btSendBuf[BUF_SIZE] = {0};
                                getPiBtMsg(&btSendBuf);
                                #ifdef ISDEBUG
                                printf("Bt True: %s\n",btSendBuf);
                                #endif
                                if((bytes_read = write(*client,&btSendBuf,BUF_SIZE)) == -1){
					close(pvsock);
					free(pvsock);
					printf("Closed BT Write\n");
					btThreadCountW --;
					pthread_exit(0);
				}
                                memset(&btSendBuf,0,BUF_SIZE);
                        }

                       /* else
                        {
                                memset(buf, 0, BUF_SIZE);
                                bytes_read = read(*client, buf, BUF_SIZE);
				printf("%s\n",buf);
                                btSendPi(buf);
                        }*/
        }
}

void *doThreadBtRead(void *pvsock)
{
	printf("BT Read_%d: Spawned\n",btThreadCountR);
	btThreadCountR++;
	int bytes_read=0;
	char buf[BUF_SIZE] = { 0 };
	int *client = (int*)pvsock;
	while(1)
	{
		memset(buf, 0, BUF_SIZE);
		if((bytes_read = read(*client, buf, BUF_SIZE)) == -1){
			close(pvsock);
			free(pvsock);
			printf("Closed BT Read\n");
			btThreadCountR --;
			pthread_exit(0);
		}
		printf("BTRAW: %s\n",buf);
		btSendPi(buf);
	}
}

void threadfuncBt()
{
	system("sdptool add --channel=4 SP");

		struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
        	char buf[BUF_SIZE] = { 0 };
        	int s, *client, bytes_read;
        	socklen_t opt = sizeof(rem_addr);

        	// allocate socket
        	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

        	// bind socket to port 1 of the first available 
        	// local bluetooth adapter
        	loc_addr.rc_family = AF_BLUETOOTH;
        	loc_addr.rc_bdaddr = *BDADDR_ANY;
        	loc_addr.rc_channel = (uint8_t) 4;
        	bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
		// put socket into listening mode
		while(1)
	{
    		listen(s, 1);
		client = (int*)malloc(sizeof(int));

    		// accept one connection
    		if((*client = accept(s, (struct sockaddr *)&rem_addr, &opt)) != -1){

    		ba2str( &rem_addr.rc_bdaddr, buf );
    		fprintf(stderr, "accepted connection from %s\n", buf);

    		// read data from the client
    		/*while(1)
		{
			if(piBtData)
			{
				char btSendBuf[BUF_SIZE] = {0};
				getPiBtMsg(&btSendBuf);
				#ifdef ISDEBUG
				printf("Bt True: %s\n",btSendBuf);
				#endif
				write(client,&btSendBuf,BUF_SIZE);
				memset(&btSendBuf,0,BUF_SIZE);
			}
			else
			{
				memset(buf, 0, BUF_SIZE);
				bytes_read = read(client, buf, BUF_SIZE);
				btSendPi(buf);
			}
		}*/

		pthread_create(&doBtThreadRead,0,&doThreadBtRead,(void*)client);
		pthread_detach(doBtThreadRead);
		pthread_create(&doBtThreadWrite,0,&doThreadBtWrite,(void*)client);
                pthread_detach(doBtThreadWrite);
		}
	}
}
