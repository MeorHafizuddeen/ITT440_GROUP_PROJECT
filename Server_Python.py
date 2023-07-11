import socket
import datetime
import os

# Function on handling client text data into combined string
def handle_client(client_socket):
    # Receive text message from the client
    data = client_socket.recv(1024).decode('utf-8').strip()
    
    # Combine the incoming text message with server current date and time as a string
    current_time = datetime.datetime.now().strftime("%d %B %Y, %H:%M")
    combined_string = f"{data}	(Received at {current_time})"
    
    # Send the combined string back to the client
    client_socket.sendall(combined_string.encode('utf-8'))
    
    # Close the connection
    client_socket.close()

# Create a socket
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
# Bind the socket to a server address on port 8484
server_address = ('', 8484)
server_socket.bind(server_address)
    
# Listen for incoming connections
server_socket.listen(5)
print('Server listening on port 8484...')
    
while True:
    # Accept a client connection
    client_socket, client_address = server_socket.accept()
    print(f'Accepted connection from {client_address}')
    
    # Fork a new process to handle the client request
    pid = os.fork()
    
    if pid == 0:  # Child process
        server_socket.close()  # Close the server socket in the child process
        handle_client(client_socket)  # Handle the client request
        os._exit(0)  # Terminate the child process
    else:
        client_socket.close()  # Close the client socket in the parent process
