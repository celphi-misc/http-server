#include "http.h"

// Size of the buffer in bytes
int size;
// File buffer
char buffer[MAX_FILE_BUFFER_SIZE];

int get_url(char *dest, const char* req)
{
    sscanf(req, "%*s %s %*s", dest);
    return strlen(dest);
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

int get_response(const char *req)
{
    char url[2048];
    get_url(url, req);
    char filename[4096];
    get_path(filename, url);
    int length = get_file_length(filename);
    if(length < 0) {
        // 404
        const char *response_str = "404 cannot GET";
        size = get_header(buffer, strlen(response_str), "text/plain", 404);
        strcpy(buffer + size, response_str);
        size += strlen(response_str);
        return 404;
    } else {
        // 200
        const char *mime = get_mime(filename);
        size = get_header(buffer, length, mime, 200);
        size += write_file_to_buffer(buffer + size, filename);
        return 200;
    }
}
