#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "crc32.h"
#include "data2file.h"
#include "file2data.h"
#include "param.h"

/* запуск с указанием имени файла в параметрах */
int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("File not specified \n");
        return 1;
    }   

    if(f2d_openfile(argv[1]) == F2D_PACKET_FILE_ERROR) return 2;
    if(param_init("list_param.cfg") != 0) return 3;  
    
    // conversion filename to *.csv
    *strstr(argv[1], ".CXE") = 0;
    strcat(argv[1], ".csv");
 
    if(d2f_open_file(argv[1]) != 0) return 4;   
    d2f_write(param_header()); 
    rec_struct rec;
    while(f2d_get_next_packet(&rec) == F2D_REC_OK) {
        d2f_write(param_data2str(&rec));
        printf("%s", param_data2str(&rec));
    }
}



