#include <iostream>
#include <cstring>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0)
    {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return -1;
    }

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }

    // Open the file to be sent
    std::ifstream inputFile("img.jpg", std::ios::binary);
    if (!inputFile)
    {
        std::cerr << "File not found" << std::endl;
        return -1;
    }

    // Send the file
    while (inputFile)
    {
        inputFile.read(buffer, BUFFER_SIZE);
        write(clientSocket, buffer, inputFile.gcount());
        memset(buffer, 0, BUFFER_SIZE);
    }
    std::cout << "File sent successfully" << std::endl;

    // Clean up and close socket
    close(clientSocket);

    return 0;
}
