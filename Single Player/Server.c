#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsa;
    SOCKET server_socket, client_socket;
    struct sockaddr_in server, client;
    int c;

    // Initialize Winsock
    printf("\nInitializing Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        printf("Failed. Error Code : %d", WSAGetLastError());
        return 1;
    }
    printf("Initialized.\n");

    // Create a socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d", WSAGetLastError());
    }
    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind
    if (bind(server_socket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("Bind failed with error code : %d", WSAGetLastError());
    }
    puts("Bind done");

    // Listen to incoming connections
    listen(server_socket, 3);

    // Accept and incoming connection
    puts("Waiting for incoming connections...");

    c = sizeof(struct sockaddr_in);
    client_socket = accept(server_socket, (struct sockaddr *)&client, &c);
    if (client_socket == INVALID_SOCKET)
    {
        printf("accept failed with error code : %d", WSAGetLastError());
    }
    puts("Connection accepted");

    // Game loop
    int client_score = 0, server_score = 0;

    char ser_win[20] = "Server wins!";
    char cli_win[20] = "Client wins!"; 
    char tie[20] = "It is a tie!";

    char winner[200];

    while (1)
    {
        char client_choice[10], server_choice[10];

        // Receive client's choice
        recv(client_socket, client_choice, 10, 0);

        // Generate random server choice
        int r = rand() % 3;
        if (r == 0)
            strcpy(server_choice, "rock");
        else if (r == 1)
            strcpy(server_choice, "paper");
        else
            strcpy(server_choice, "scissors");

        // Determine winner and update scores
        if (strcmp(client_choice, "rock") == 0)
        {
            if (strcmp(server_choice, "rock") == 0){
                printf("Generated: ROCK\n");
                printf("Tie!\n");
                printf("SCORE - Server : %d ; Client : %d\n\n", server_score, client_score);
                send(client_socket, tie, strlen(tie), 0);
            }
            else if (strcmp(server_choice, "paper") == 0)
            {
                printf("Generated: PAPER\n");
                printf("Server wins!\n");
                server_score++;
                printf("SCORE - Server : %d ; Client : %d\n\n", server_score, client_score);
                send(client_socket, ser_win, strlen(ser_win), 0);
            }
            else
            {
                printf("Generated: SCISSORS\n");
                printf("Client wins!\n");
                client_score++;
                printf("SCORE - Server : %d ; Client : %d\n\n", server_score, client_score);
                send(client_socket, cli_win, strlen(cli_win), 0);
            }
        }
        else if (strcmp(client_choice, "paper") == 0)
        {
            if (strcmp(server_choice, "rock") == 0)
            {
                printf("Generated: ROCK\n");
                printf("Client wins!\n");
                client_score++;
                printf("SCORE - Server : %d ; Client : %d\n\n", server_score, client_score);
                send(client_socket, cli_win, strlen(cli_win), 0);
            }
            else if (strcmp(server_choice, "paper") == 0){
                printf("Generated: PAPER\n");
                printf("Tie!\n");
                printf("SCORE - Server : %d ; Client : %d\n\n", server_score, client_score);
                send(client_socket, tie, strlen(tie), 0);
            }
            else
            {
                printf("Generated: SCISSORS\n");
                printf("Server wins!\n");
                server_score++;
                printf("SCORE - Server : %d ; Client : %d\n\n", server_score, client_score);
                send(client_socket, ser_win, strlen(ser_win), 0);
            }
        }
        else
        {
            if (strcmp(server_choice, "rock") == 0)
            {
                printf("Generated: ROCK\n");
                printf("Server wins!\n");
                server_score++;
                printf("SCORE - Server : %d ; Client : %d\n\n", server_score, client_score);
                send(client_socket, ser_win, strlen(ser_win), 0);
            }
            else if (strcmp(server_choice, "paper") == 0)
            {
                printf("Generated: PAPER\n");
                printf("Client wins!\n");
                client_score++;
                printf("SCORE - Server : %d ; Client : %d\n\n", server_score, client_score);
                send(client_socket, cli_win, strlen(cli_win), 0);
            }
            else{
                printf("Generated: SCISSORS\n");
                printf("Tie!\n");
                printf("SCORE - Server : %d ; Client : %d\n\n", server_score, client_score);
                send(client_socket, tie, strlen(tie), 0);
            }
        }

        // Send current scorecard to client
        // char score[50];
        // sprintf(score, "Client: %d ; Server: %d", client_score, server_score);
        // send(client_socket, score, strlen(score), 0);

        // Check for winner
        if (client_score == 5)
        {
            printf("\nCLIENT HAS WON THE GAME\n");
            strcpy(winner, "Client has won the game");
            send(client_socket, winner, sizeof(winner), 0);
            break;
        }
        else if (server_score == 5)
        {
            printf("\nSERVER HAS WON THE GAME\n");
            strcpy(winner, "Server has won the game");
            send(client_socket, winner, sizeof(winner), 0);
            break;
        }

         char score[50];
        sprintf(score, "Client: %d ; Server: %d", client_score, server_score);
        send(client_socket, score, strlen(score), 0);
    }

    // Close the socket and cleanup
    printf("\n\n***SERVER CLOSING***\n");
    closesocket(client_socket);
    WSACleanup();

    return 0;
}