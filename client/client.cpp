#include <iostream>
#include <cstring>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main()
{
    int serverSocket, newSocket, valRead;
    struct sockaddr_in serverAddr;
    int opt = 1;
    int addrlen = sizeof(serverAddr);
    char buffer[BUFFER_SIZE] = {0};

    // Creating socket file descriptor
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        std::cerr << "Setsockopt failed" << std::endl;
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Binding socket to the specified address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    // Listening for incoming connections
    if (listen(serverSocket, 3) < 0)
    {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    // Accepting incoming connection
    if ((newSocket = accept(serverSocket, (struct sockaddr *)&serverAddr, (socklen_t *)&addrlen)) < 0)
    {
        std::cerr << "Accept failed" << std::endl;
        return -1;
    }

    // Receiving the file
    std::ofstream outputFile("img.jpg", std::ios::binary);
    while ((valRead = read(newSocket, buffer, BUFFER_SIZE)) > 0)
    {
        outputFile.write(buffer, valRead);
        memset(buffer, 0, BUFFER_SIZE);
    }
    std::cout << "File received successfully" << std::endl;

    // Clean up and close sockets
    close(newSocket);
    close(serverSocket);

    return 0;
}
