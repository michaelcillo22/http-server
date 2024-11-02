#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    
    
    // HTTP response
    const char *http_response = "HTTP/1.1 200 OK\r\n"
                                "Content-Type: text/html\r\n"
                                "Content-Length: 50\r\n"
                                "\r\n"
                                "<html><body><h1>Hello, World!</h1></body></html>";
                                
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) ==0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Bind to address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Listening on port %d...\n", PORT);
    
    while (1) {
        // Accept an incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept connection failed");
            close(server_fd);
            exit(EXIT_FAILURE);
        }
        
        // Read the client request
        read(new_socket, buffer, 1024);
        printf("Received request:\n%s\n", buffer);

        // Send HTTP response
        send(new_socket, http_response, strlen(http_response), 0);
        printf("HTTP response sent.\n");

        // Close the socket
        close(new_socket);     
    }   
    
    // Close the server socket
    close(server_fd);
    return 0; 
}