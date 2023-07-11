import socket
import time

# Get server IP address and port number from user input
server_ip = input("\nEnter the server IP address: ")
server_port = int(input("Enter the server port number: "))
    
# Create a socket
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
# Connect to the server
server_address = (server_ip, server_port)
client_socket.connect(server_address)
print(f'\nConnected to {server_address}')
    
# Receive input string from user
user_input = input("\nEnter a message: ")
    
# Send the user input to the server
client_socket.sendall(user_input.encode('utf-8'))
print(f'\nSending message to {server_address}...')
time.sleep(5) #Sleep for 5 seconds to indicate transmission processing

# Receive the reply from the server
reply = client_socket.recv(1024).decode('utf-8')
    
# Print the incoming reply string from the server
print(f'\nReply from server: {reply}')
    
# Close the connection
client_socket.close()
