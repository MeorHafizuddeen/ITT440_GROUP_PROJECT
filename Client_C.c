#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

// Macro definition
#define BUFFER_SIZE 1024

// Main function
int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];
    
    // Get server IP address and port number from user input
    char server_ip[16];
    int server_port;
    printf("\nEnter the server IP address: ");
    scanf("%s", server_ip);
    printf("Enter the server port number: ");
    scanf("%d", &server_port);
    
    // Create a socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("\nSocket creation failed");
        return 0;
    }
    
    // Connect to the server
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(server_ip);
    server_address.sin_port = htons(server_port);
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("\nConnection failed");
        return 0;
    }
    printf("\nConnected to %s:%d\n", server_ip, server_port);
    
    // Receive input string from user
    printf("\nEnter a message: ");
    getchar();  // Clear input buffer
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';  // Remove newline character

    // Send the user input to the server
    send(client_socket, buffer, strlen(buffer), 0);
    printf("\nSending message to %s...\n", server_ip);
    sleep (5); //Sleep for 5 seconds to indicate text transmission from client to server is processing

    // Receive the reply from the server
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    
    // Print the incoming reply string from the server
    printf("\nReply from server: %s\n", buffer);
    
    // Close the connection
    close(client_socket);
    
    return 0;
}
