#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "crc32.h"
#include "file2data.h"
#include "param.h"

/* запуск с указанием имени файла в параметрах */
int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("File not specified\n");
        return 1;
    }   

    if(strstr(argv[1], ".CXE") == NULL) {
        printf("the file must be *.CXE \n");
        return 2;
    }
    
    printf("packet type: %d \n", f2d_openfile(argv[1]));
    //param_init("list_param.cfg");  
    param_init_new("list_param.cfg");

// conversion filename to *.csv
    *strstr(argv[1], ".CXE") = 0;
    strcat(argv[1], ".csv");
 
    FILE *ofile = fopen(argv[1], "w");
    if(ofile == NULL) {
        printf("failed to create file\n");
        return 4;
    }
   
    rec_struct rec;
    while(f2d_get_next_packet(&rec) == F2D_REC_OK) {
        printf("%02d:%02d:%02d vmin: %d vmax: %d \n", rec.bat.hours, rec.bat.minutes, rec.bat.seconds, rec.mb[0].cell_volt_min, rec.mb[0].cell_volt_max);
        printf("%4d_%4d_%4d_%4d_%4d \n", rec.cell[0][0][0].volt, 
                                         rec.cell[0][5][6].volt,
                                         rec.cell[0][5][7].volt,
                                         rec.cell[0][5][8].volt,
                                         rec.cell[0][13][9].volt);
        //printf("%s", param_data2str(&rec));


        break;                                         
    }

    





    printf("end \n");
    fclose(ofile);
}



