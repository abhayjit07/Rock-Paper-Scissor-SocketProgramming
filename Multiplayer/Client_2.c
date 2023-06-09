#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char *argv[])
{
    WSADATA wsa;
    SOCKET socket_client;
    struct sockaddr_in server;
    char move[10], result[200];

    printf("Initializing Winsock...\n");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");

    // Create a socket
    if ((socket_client = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);

    // Connect to remote server
    if (connect(socket_client, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("Connect failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Connected.\n");

    while (1)
    {
        printf("Make your move (rock, paper, or scissors): ");
        scanf("%s", move);
        if(strcmp(move, "rock") != 0 && strcmp(move, "scissors") != 0 && strcmp(move, "paper") != 0){
            printf("\nInvalid Input! Enter rock, paper or scissors!\n");
            continue;
        }
        send(socket_client, move, sizeof(move), 0);
        recv(socket_client, result, sizeof(result), 0);

        if(strcmp(result, "\nPlayer 1 has WON the game by reaching 5 points first!\nThank You for playing") == 0){
            printf("\n%s\n\n", result);
            break;
        }

        else if(strcmp(result, "\nPlayer 2 has WON the game by reaching 5 points first!\nThank You for playing") == 0){
            printf("\n%s\n\n", result);
            break;
        }

        printf("\n%s\n\n", result);
    }

    printf("\n***CLIENT 2 CLOSING***\n\n");

    closesocket(socket_client);
    WSACleanup();
    return 0;
}