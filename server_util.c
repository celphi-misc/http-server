#include "server_util.h"

struct sockaddr_in server;

// Create a new socket on the referred port number
// The return value is either an error code (negative)
// or a socket descriptor.
int new_socket(uint16_t port)
{
    // Create socket
    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc == -1)
    {
#ifdef SERVER_OUTPUT
        perror("Socket creation failed");
#endif
        return FAILED_SOCKET_CREATION;
    }
#ifdef SERVER_OUTPUT
    puts("Socket created.");
#endif
    // sockaddr_in strcuture
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    // Reuse addr
    int on = 1;
    if(setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))< 0)
    {
#ifdef SERVER_OUTPUT
        perror("Set socket option failed");
#endif
        return(FAILED_SOCKET_CREATION);
    }

    // Binding
    if(bind(socket_desc, (struct sockaddr*)&server, sizeof(server)))
    {
#ifdef SERVER_OUTPUT
        perror("Binding failed");
#endif
        return FAILED_BINDING;
    }
#ifdef SERVER_OUTPUT
    puts("Binded");
#endif

    // Listening
    listen(socket_desc, 3);
    return socket_desc;
}

// The argument is the socket descriptor
int accept_client(int sock_desc)
{
    int client_sock;
    struct sockaddr_in client;
    int addrlen = sizeof(struct sockaddr_in);

    // Accept a connection
    if((client_sock = accept(sock_desc, (struct sockaddr*)&client, (socklen_t*)&addrlen)))
    {
        return client_sock;
    }
    return client_sock;
}
