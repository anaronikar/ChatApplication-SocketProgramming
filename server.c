#include<stdio.h>
#include<stdlib.h>
#include<winsock2.h>
#include<ws2tcpip.h>
#include<sys/types.h>
#include<time.h>

#pragma comment(lib,"ws2_32.lib") //pragma-directive used to link the winsock library

int main()
{
    char client_message[1000];
    int read_size;
    char message2[1000];

    time_t t;
    time(&t);

    //to initialize winsock
    WSADATA wsa;
	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
	{
		printf("\nFailed. Error Code : %d",WSAGetLastError());
		return 1;
	}
	printf("Initialised at %s ",ctime(&t));
    printf("\n");


    char server_message[256]="\n THIS MESSAGE IS SENT BY SERVER TO THE CLIENT. \n";

    //1.create  the server socket
    int server_socket;
    if((server_socket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d" , WSAGetLastError());
	}

	printf("\nSocket created at %s \n",ctime(&t));
    printf("\n");


    // 2.define the server ip-address and port
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(9002);
    server_address.sin_addr.s_addr=INADDR_ANY;


    //3.bind the socket to our specified IP and port
    int i = bind(server_socket, (struct sockaddr*) &server_address,sizeof(server_address));

    if (i < 0)
        printf("Bind failed\n");
    else
        printf("Bind successful at %s \n",ctime(&t));

    printf("\n");

    //4.listen to any connections coming from client
    if(listen(server_socket,5) < 0)
    { 
        printf("Failed to listen\n");
        return -1;
    }
    else
    {
        printf("Listen successful at %s \n",ctime(&t));
        printf("\n");
        printf("Waiting for incoming connections...\n");
    }

    printf("\n");

    //5.accept incoming connect request from client
    int client_socket;
    client_socket=accept(server_socket,NULL,NULL);
    if(client_socket == INVALID_SOCKET)
		printf("Could not accept socket request: %d" , WSAGetLastError());
    else
	    printf("Socket request accepted at %s \n",ctime(&t));

    printf("\n");

    //6.get into the send/receive loop with client (in an infinite loop)
    while(1)
	{
        //Receive a message from client
	    while( (read_size = recv(client_socket , client_message , 1000 , 0)) > 0 )
	    {
	    	//Send the message back to client
            printf("\nMessage received from client at: %s ",ctime(&t));
            puts(client_message);
            printf("\n");
            printf("\nEnter response to be sent to client : \n");
		    scanf("%s" , message2);
	    	send(client_socket , message2 , strlen(message2),0);
        }

	    if(read_size == 0)
	    {
		    printf("\nClient disconnected\n");
		    fflush(stdout);
	    }
	    else if(read_size == -1)
	    {
		    printf("\nNothing received from the server.\n");
            printf("\nThe time is currently: %s \n",ctime(&t));
	    }
        char client_message[1000]={};
        char message2[1000]={};
    }

    /*
    //send the message
    send(client_socket,server_message,sizeof(server_message),0);
    */

    //close the socket
    closesocket(server_socket);
    WSACleanup(); //used to terminate the use of winsock2 library (ws2_32)
    return(0);
}