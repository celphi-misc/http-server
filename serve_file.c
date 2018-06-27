#include "serve_file.h"

// File server static path
const char *static_path = "./static";

// Get mime type
const char* get_mime(const char* filename)
{
    // Consult magic
    magic_t magic = magic_open(MAGIC_MIME_TYPE);
    magic_load(magic, NULL);
    magic_compile(magic, NULL);
    const char* mime = magic_file(magic, filename);
    if(!strcmp(mime, "text/plain"))
    {
        const char* last = filename + strlen(filename) - 1;
        const char* dot_ch;
        for(dot_ch = last; dot_ch >= filename; dot_ch--)
            if(*dot_ch == '.') break;
        if(!strcmp(dot_ch, ".json"))    return "application/json; charset=utf-8";
        if(!strcmp(dot_ch, ".js"))      return "application/javascript; charset=utf-8";
        if(!strcmp(dot_ch, ".es"))      return "application/ecmascript; charset=utf-8";
        if(!strcmp(dot_ch, ".xml"))     return "application/xml; charset=utf-8";
        if(!strcmp(dot_ch, ".xhtml"))   return "application/xhtml+xml; charset=utf-8";
        if(!strcmp(dot_ch, ".css"))     return "text/css; charset=utf-8";
        if(!strcmp(dot_ch, ".html"))    return "text/html; charset=utf-8";
        if(!strcmp(dot_ch, ".htm"))     return "text/html; charset=utf-8";
        else return "text/plain; charset=utf-8";
    }
    return mime;
}

// Get file path by url
int get_path(char* dest, const char* url)
{
    strcpy(dest, static_path);
    return strlen(strcat(dest, url));
}

int set_static_path(const char *path)
{
    static_path = path;
    return 1;
}

const char* get_static_path()
{ return static_path; }

int get_file_length(const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if(f)
    {
        fseek(f, 0, SEEK_END);
        int length = ftell(f);
        fclose(f);
        return length;
    }
    else return -1;
}

int write_file_to_buffer(char *dest, const char *filename)
{
    FILE *f = fopen(filename, "rb");
    if(f)
    {
        fseek(f, 0, SEEK_END);
        int length = ftell(f);
        fseek(f, 0, SEEK_SET);
        fread(dest, 1, length, f);
        fclose(f);
        return length;
    }
    else return -1;
}
