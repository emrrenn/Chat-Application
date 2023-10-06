#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

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
    socklen_t client_addr_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if(client_socket == -1){
        std::cerr<<"Error accepting connection"<<std::endl;
        return -1;
    }
    std::cout<<"Client connected"<<std::endl;

    // Close the sockets
    close(client_socket);
    close(server_socket);
    return 0;
}
