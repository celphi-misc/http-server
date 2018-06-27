#include "server_util.h"
#include "http.h"

int main(int argc, char** argv)
{
    int server_sock = new_socket(8080);
    int client_sock = accept_client(server_sock);
    char recv_buf[4096];
    int read_size = recv(client_sock, recv_buf, 4096, 0);
    puts(recv_buf);
    get_response(recv_buf);
    write(client_sock, get_buffer(), get_buffer_size());
    puts(get_buffer());
    return 0;
}
