#ifndef SERVE_FILE_CELPHI_H__
#define SERVE_FILE_CELPHI_H__

#include "commons.h"
#include <string.h>

#ifdef USING_MAGIC_MIME
#include <magic.h>
#endif

const char* get_mime(const char* filename);

int set_static_path(const char *path);
const char* get_static_path();
int get_path(char *dest, const char *url);

int get_file_length(const char *filename);

int write_file_to_buffer(char *dest, const char *filename);

#endif
