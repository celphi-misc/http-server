// This file serves as the interface of the server utility methods
#ifndef __CELPHI_SERVER_UTIL_H
#define __CELPHI_SERVER_UTIL_H

#include "commons.h"

typedef struct package package;
struct package {
    int client_sock;
    char *recv_buf;
};

// Methods

// Create a new socket on the referred port number
// The return value is either an error code (negative)
// or a socket descriptor.
int new_socket(uint16_t port);
// Block to wait for accepting client connection
// The return value is either an error code (negative)
// or the client socket descriptor.
int accept_client(int server_sock_desc);

#endif
