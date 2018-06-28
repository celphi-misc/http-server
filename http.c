#include "http.h"

// Size of the buffer in bytes
int size;
// File buffer
char buffer[MAX_FILE_BUFFER_SIZE];
// Buffer mutex
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;
// Prefix
const char *prefix = "";

HttpMethod get_url(char *dest, const char* req)
{
    char method[10];
    sscanf(req, "%s %s %*s", method, dest);
    if(!strcmp(method, "POST")) return POST;
    return GET;
}

const char* get_status(unsigned status)
{
    switch(status)
    {
        case 100: return "Continue";
        case 101: return "Switching Protocols";
        case 200: return "OK";
        case 201: return "Created";
        case 202: return "Accepted";
        case 203: return "Non-Authoritative Information";
        case 204: return "No Content";
        case 205: return "Reset Content";
        case 206: return "Partial Content";
        case 300: return "Multiple Choices";
        case 301: return "Moved Permanently";
        case 302: return "Found";
        case 303: return "See Other";
        case 304: return "Not Modified";
        case 305: return "Use Proxy";
        case 307: return "Temporary Redirect";
        case 400: return "Bad Request";
        case 401: return "Unauthorized";
        case 402: return "Payment Required";
        case 403: return "Forbidden";
        case 404: return "Not Found";
        case 405: return "Method Not Allowed";
        case 406: return "Not Acceptable";
        case 407: return "Proxy Authentication Required";
        case 408: return "Request Time-out";
        case 409: return "Conflict";
        case 410: return "Gone";
        case 411: return "Length Required";
        case 412: return "Precondition Failed";
        case 413: return "Request Entity Too Large";
        case 414: return "Request-URI Too Large";
        case 415: return "Unsupported Media Type";
        case 416: return "Requested range not satisfiable";
        case 417: return "Expectation Failed";
        case 500: return "Internal Server Error";
        case 501: return "Not Implemented";
        case 502: return "Bad Gateway";
        case 503: return "Service Unavailable";
        case 504: return "Gateway Time-out";
        case 505: return "HTTP Version not supported";
        default: return "Internal Server Error";
    }
}

int get_header(char *dest, 
    unsigned length, const char *mime, unsigned status)
{
    char time_str[100];
    time_t now = time(0);
    struct tm tm = *gmtime(&now);
    strftime(time_str, sizeof(time_str), "%a, %d %b %Y %H:%M:%S %Z", &tm);
    return sprintf(dest, "\
HTTP/1.1 %u %s\r\n\
Date: %s\r\n\
Content-Type: %s\r\n\
Content-Length: %u\r\n\
Server: MyServer/0.0 (Unix) (Deepin/Linux)\r\n\
Connection: Keep-Alive\r\n\r\n",
    status, get_status(status), time_str, mime, length);
}

const char* get_buffer()
{ return buffer; }

int get_buffer_size()
{ return size; }

int set_prefix(const char* prefix_)
{ prefix = prefix_; return 1; }

const char* get_prefix()
{ return prefix; }

int handle_get(const char* temp_url)
{
    int response404 = 0;
    const char* url = strstr(temp_url, prefix) + strlen(prefix);

    // Response 404 if the file path is not the mapped location
    if(url < temp_url) response404 = 1;
    url = url > temp_url ? url : temp_url;
    
    printf("GET "); puts(url);
    char filename[4096];
    get_path(filename, url);
    int length = get_file_length(filename);

    int status;
    
    // Critical section
    pthread_mutex_lock(&buffer_mutex);
    if(length < 0 || response404) {
        // 404
        const char *response_str = "<html><body>404 cannot GET</body></html>";
        size = get_header(buffer, strlen(response_str), "text/html; charset=utf-8", 404);
        strcpy(buffer + size, response_str);
        size += strlen(response_str);
        status = 404;
    } else {
        // 200
        const char *mime = get_mime(filename);
        size = get_header(buffer, length, mime, 200);
        size += write_file_to_buffer(buffer + size, filename);
        status = 200;
    }
    pthread_mutex_unlock(&buffer_mutex);
    
    return status;
}

int handle_post(const char* temp_url, const char* body)
{
    int response404 = 0;
    const char* url = strstr(temp_url, prefix) + strlen(prefix);

    // Response 404 if the file path is not the mapped location
    if(url < temp_url) response404 = 1;
    url = url > temp_url ? url : temp_url;
    
    printf("POST "); puts(url);

    int status;

    // Critical section
    pthread_mutex_lock(&buffer_mutex);
    if(strcmp(url, "/dopost") || response404) {
        // 404
        const char *response_str = "<html><body>404 cannot POST</body></html>";
        size = get_header(buffer, strlen(response_str), "text/html; charset=utf-8", 404);
        strcpy(buffer + size, response_str);
        size += strlen(response_str);
        status = 404;
    } else {
        // 200
        const char *success_str = "<html><body>User login succeeded</body></html>";
        const char *failure_str = "<html><body>User login failed</body></html>";
        const char *response_str;
        puts(body);
        if(strcmp(body, "login="LOGIN_STR"&pass="PASS_STR))
            response_str = failure_str;
        else response_str = success_str;
        size = get_header(buffer, strlen(response_str), "text/html; charset=utf-8", 200);
        strcpy(buffer + size, response_str);
        size += strlen(response_str);
        status = 200;
    }
    pthread_mutex_unlock(&buffer_mutex);
    
    return status;
}

int get_response(const char *req)
{
    char temp_url[2048];
    
    const char* body = get_body(req);

    int method = get_url(temp_url, req);

    if(method == GET)
        return handle_get(temp_url);
    else
        return handle_post(temp_url, body);    
}

const char* get_body(const char* req)
{
    return strstr(req, "\r\n\r\n")+4;
}