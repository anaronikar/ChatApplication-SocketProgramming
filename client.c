#include<stdio.h>
#include<stdlib.h>
#include<ws2tcpip.h>
#include<sys/types.h>
#include<winsock2.h>
#include<time.h>

#pragma comment(lib,"ws2_32.lib") //pragma-directive used to link the winsock library

int main()
{
    char message[1000], server_reply[1000];
    
    time_t t;
    time(&t);


    //to initialize winsock
    WSADATA wsa;
	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("Failed. Error Code : %d",WSAGetLastError());
		return 1;
	}
	printf("Initialised at %s \n",ctime(&t));
    printf("\n");

    //1.create a socket
    int network_sock;
    if((network_sock = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("Socket created at %s \n",ctime(&t));
    printf("\n");


    //2.define server ip-address and port
    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_port=htons(9002);
    //server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1");

    //3.connect to server socket on local host
    int connection_status=connect(network_sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if(connection_status == SOCKET_ERROR)
	{
		printf("Could not connect socket : %d" , WSAGetLastError());
        connection_status = closesocket(network_sock);
        if (connection_status == SOCKET_ERROR)
            printf("Closesocket function failed with error: %ld\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }

    printf("\nConnected to server!\n");

    //4. enter into a send/receive infinite loop with server
	while(1)
	{
		printf("\nEnter message to be sent to server : \n");
		scanf("%s" , message);
		
		//send entered message
		if(send(network_sock, message , strlen(message) , 0) < 0)
		{
			printf("\nSend failed\n");
			return 1;
		}
		
		//receive a reply from the server
		if(recv(network_sock , server_reply , 1000 , 0) < 0)
		{
			printf("\nRecieving of the message has failed\n");
			break;
		}
		
		printf("\nReply received from Server at: %s \n",ctime(&t));
		puts(server_reply);

        char server_reply[1000]={};
        char message[1000]={};
	}

    /*
    //sending or receiving data from a server - only once
    char server_resp[260];
    recv(network_sock, server_resp, sizeof(server_resp),0);

    //printing out the data
    printf("\nThe data received from server:%s\n",server_resp);
    */


    //close the socket
    closesocket(network_sock);
    WSACleanup(); //used to terminate the use of winsock2 library (ws2_32)
    return 0;
}