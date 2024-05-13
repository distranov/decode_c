#ifndef _DATA2FILE_H_
#define _DATA2FILE_H_

#include <stdint.h>

uint8_t d2f_open_file(const char *filename);
void d2f_write(const char *str);
void d2f_close_file(void);

#endif