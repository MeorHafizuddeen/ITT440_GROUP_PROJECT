#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

// Macros definition
#define PORT 8484
#define BUFFER_SIZE 1024

// Function on handling client text data into combined string
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;
    
    // Receive text message from the client
    bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
    buffer[bytes_received] = '\0';
    
    // Combine the incoming text message with server current date and time as a string
    time_t current_time = time(NULL);
    char time_string[100];
    strftime(time_string, sizeof(time_string), "%d %B %Y, %H:%M", localtime(&current_time));
    sprintf(buffer, "%s	(Received at %s)", buffer, time_string);
    
    // Send the combined string back to the client
    send(client_socket, buffer, strlen(buffer), 0);
    
    // Close the connection
    close(client_socket);
}

// Main function
int main() {
    //Declaration of variables involves with socket creation and port binding
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_length = sizeof(client_address);
    int pid;
    
    // Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return -1;
    }
    
    // Bind the socket to server address on port 8484
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Socket binding failed");
        return -1;
    }
    
    // Listen for incoming connections
    if (listen(server_socket, 5) == -1) {
        perror("Listening failed");
        return -1;
    }
    printf("Server listening on port %d...\n", PORT);
    
    while (1) {
        // Accept a client connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_length);
        if (client_socket == -1) {
            perror("Acceptance failed");
            return -1;
        }
        printf("Accepted connection from %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        
        // Fork a new process to handle the client request
        pid = fork();
        
        if (pid == 0) {  // Child process
            close(server_socket);  // Close the server socket in the child process
            handle_client(client_socket);  // Handle the client request
            exit(0);  // Terminate the child process
        } else {
            close(client_socket);  // Close the client socket in the parent process
        }
    }
    
    // Close the server socket
    close(server_socket);
    
    return 0;
}
