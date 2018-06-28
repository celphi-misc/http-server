#include "server_util.h"
#include "http.h"
#include "pthread.h"

int server_should_stop = 0;

void *start_serve(void *client_sock)
{
    char recv_buf[4096];
    int sock = *(int*)client_sock;
    recv(sock, recv_buf, 4096, 0);
    int status = get_response(recv_buf);
    write(sock, get_buffer(), get_buffer_size());
    close(sock);
    printf("Server response %d\n", status);
    pthread_exit(0);
}

void *start_accept(void *server_sock)
{
    while(!server_should_stop)
    {
        int client_sock = accept_client(*(int*)server_sock);
        if(client_sock < 0)
        {
            printf("Accept Fault: %d\n", client_sock);
            pthread_exit(0);
        }

        pthread_t  pid;
        pthread_create(&pid, NULL, start_serve, &client_sock);
        pthread_detach(pid);
    }
    pthread_exit(0);
}

int main(int argc, char** argv)
{
    const char* static_dir = "static";
    int port = 4531;
    const char* prefix = "";
    if(argc >= 2)
        static_dir = argv[1];
    if(argc >= 3)
        sscanf(argv[2], "%d", &port);
    if(argc >= 4)
        prefix = argv[3];
    set_static_path(static_dir);
    set_prefix(prefix);
    int server_sock = new_socket(port);
    pthread_t pid;
    void *result;
    pthread_create(&pid, NULL, start_accept, &server_sock);
    pthread_join(pid, &result);
    return 0;
}
