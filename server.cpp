#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 5555

int main() {

    //Create a socket
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1){
        std::cerr<<"Error creating a socket"<<std::endl;
        return -1;
    }

    //Define the server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    //Bind the socket to address and port
    if((bind(server_socket, (const struct sockaddr*)&server_addr, sizeof(server_addr))) == -1){
        std::cerr<<"Error binding the socket"<<std::endl;
        return -1;
    }

    //Listen for incoming connections
    if(listen(server_socket, 5) == -1){
        std::cerr<<"Error listening"<<std::endl;
        return -1;
    }
    std::cout<<"Server listening on port " << PORT << "..." <<std::endl;

    //Accept a client connection
    int client_socket;
    struct sockaddr_in client_addr;
    client_addr.sin_port = htons(PORT);
    socklen_t client_addr_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if(client_socket == -1){
        std::cerr<<"Error accepting connection"<<std::endl;
        return -1;
    }
    std::cout << "Accepted connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << std::endl;

    //Chat with the Client
    while(true){
        char buffer[1024];
        ssize_t  bytesRead = recv(client_socket, buffer, sizeof(buffer), 0);
        buffer[bytesRead] = '\0';
        std::cout << "Client: " << buffer << std::endl;

        char message[1024];
        std::cout << "Server: ";
        std::cin.getline(message, sizeof(message));
        if (strcmp(message, "exit") == 0) {
            char exit_msg[] = "server ended the chat";
            send(client_socket, exit_msg, strlen(exit_msg), 0);
            break;
        }
        send(client_socket, message, strlen(message), 0);
    }
    // Close the sockets
    std::cout<<"CLosing sockets"<<std::endl;
    close(client_socket);
    close(server_socket);
    return 0;
}
