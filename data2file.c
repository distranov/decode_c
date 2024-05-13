#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "data2file.h"


static FILE *ofile = NULL;
//-----------------------------------------------------------------------------
uint8_t d2f_open_file(const char *filename) {
    ofile = fopen(filename, "w");
    if(ofile == NULL) {
        printf("failed to create file\n");
        return 4;
    }
    return 0;

}
//-----------------------------------------------------------------------------
void d2f_write(const char *str) {
    if(ofile == NULL) return;
    fputs(str, ofile);
}
//-----------------------------------------------------------------------------
void d2f_close_file(void) {
    if(ofile != NULL)
        fclose(ofile);
}