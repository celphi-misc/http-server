#ifndef SERVE_FILE_CELPHI_H__
#define SERVE_FILE_CELPHI_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <magic.h>

const char* get_mime(const char* filename);

int set_static_path(const char *path);
const char* get_static_path();
int get_path(char *dest, const char *url);

int get_file_length(const char *filename);

int write_file_to_buffer(char *dest, const char *filename);

#endif
