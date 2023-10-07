#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5555

int main(){
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1){
        std::cerr<<"Error creating a socket"<<std::endl;
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if((connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr))) == -1)
    {
        std::cerr<<"Error connecting to server"<<std::endl;
        return -1;
    }
    std::cout<<"Connected to server"<<std::endl;
    std::cout<<"Type exit to quit."<<std::endl;

    char message[1024];

    //Chat with the server
    while(true){
        std::cout << "Client: ";
        std::cin.getline(message, sizeof(message));
        if (strcmp(message, "exit") == 0) {
            char exit_msg[] = "client ended the chat";
            send(client_socket, exit_msg, strlen(exit_msg), 0);
            break;
        }
        send(client_socket, message, strlen(message), 0);

        char buffer[1024];
        ssize_t bytesRead = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytesRead == -1) {
            std::cerr << "Error receiving data" << std::endl;
            close(client_socket);
            return 1;
        }
        //buffer[bytesRead] = '\0';
        std::cout << "Server: " << buffer << std::endl;
    }
    //Close the socket
    std::cout<<"Closing socket"<<std::endl;
    close(client_socket);
    return 0;
}