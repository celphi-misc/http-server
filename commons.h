// Definitions of the client and the server that are in common
#ifndef __CELPHI_COMMONS_H
#define __CELPHI_COMMONS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

// Toggle server output
#define SERVER_OUTPUT
#define MAX_FILE_BUFFER_SIZE 0xfffff

// Exit code definations
#define SUCCEED_EXITCODE         0
#define FAILED_HOSTNAME         -1
#define FAILED_SOCKET_CREATION  -2
#define FAILED_BINDING          -3
#define FAILED_CONNECTING       -4
#define FAILED_SENDING          -5

#define LOGIN_STR               "3150104531"
#define PASS_STR                "4531"

#endif