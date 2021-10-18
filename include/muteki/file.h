#ifndef __MUTEKI_FILE_H__
#define __MUTEKI_FILE_H__

#include <muteki/common.h>

extern void *_afopen(const char *pathname, const char *mode);
extern bool _fread(void *ptr, size_t size, size_t nmemb, void *stream);
extern bool _fwrite(const void *ptr, size_t size, size_t nmemb, void *stream);
extern int _fclose(void *stream);

#endif
