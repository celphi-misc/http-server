#ifndef HTTP_CELPHI_H__
#define HTTP_CELPHI_H__

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "serve_file.h"
#define MAX_FILE_BUFFER_SIZE 0xfffff

int get_url(char *dest, const char *req);
int get_header(char *dest, unsigned length, const char *mime, unsigned status);
int get_response(const char *req);

// You should:
// Try to read the file first, then you can get
// buffer and its size
int read_file(const char* filename);
const char* get_buffer();
int get_buffer_size();

#endif
