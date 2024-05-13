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
        printf("File not specified\n");
        return 1;
    }   

    if(strstr(argv[1], ".CXE") == NULL) {
        printf("the file must be *.CXE \n");
        return 2;
    }
    
    printf("packet type: %d \n", f2d_openfile(argv[1]));
    param_init("list_param.cfg");  
    
// conversion filename to *.csv
    *strstr(argv[1], ".CXE") = 0;
    strcat(argv[1], ".csv");
 
    FILE *ofile = fopen(argv[1], "w");
    if(d2f_open_file(argv[1]) != 0) return 4;


   
    d2f_write(param_header()); 
    rec_struct rec;
    char s[100];
    while(f2d_get_next_packet(&rec) == F2D_REC_OK) {

        
        printf("time %02d:%02d:%04d %06d \r\n", rec.bat.hours, rec.bat.minutes, rec.bat.seconds, rec.bat.dm1_spn);
        //d2f_write(s);

        //param_data2str(&rec);
        //printf("%s", param_data2str(&rec));

        d2f_write(param_data2str(&rec));

        //printf("\n");
        //printf("%02d:%02d:%02d vmin: %d vmax: %d \n", rec.bat.hours, rec.bat.minutes, rec.bat.seconds, rec.mb[0].cell_volt_min, rec.mb[0].cell_volt_max);
        /*
        printf("%4d_%4d_%4d_%4d_%4d \n", rec.cell[0][0][0].volt, 
                                         rec.cell[0][5][6].volt,
                                         rec.cell[0][5][7].volt,
                                         rec.cell[0][5][8].volt,
                                         rec.cell[0][13][9].volt);
                                         */
        //printf("%s", param_data2str(&rec));


        //break;                                         
    }
 /*  
    printf("offset bub_date: %04d \n", (uint32_t)&rec.bat.date - (uint32_t)&rec);
    printf("offset mb_cur[0]: %04d \n", (uint32_t)&rec.mb[0].cur - (uint32_t)&rec);
    printf("offset mb_cur[1]: %04d \n", (uint32_t)&rec.mb[1].cur - (uint32_t)&rec);

    printf("offset sma_tempA[0][0]: %04d \n", (uint32_t)&rec.sma[0][0].tempA - (uint32_t)&rec);
    

    printf("offset sma_tempA[0][12]: %04d \n", (uint32_t)&rec.sma[0][12].tempA - (uint32_t)&rec);
    printf("offset sma_tempA[0][13]: %04d \n", (uint32_t)&rec.sma[0][13].tempA - (uint32_t)&rec);
    
    printf("offset cell_volt[0][0][0]: %04d \n", (uint32_t)&rec.cell[0][0][0].volt - (uint32_t)&rec);    
    printf("offset cell_volt[0][5][4]: %04d \n", (uint32_t)&rec.cell[0][5][4].volt - (uint32_t)&rec);
    printf("offset cell_temp[0][5][4]: %04d \n", (uint32_t)&rec.cell[0][5][4].temp - (uint32_t)&rec);
*/

/*
    printf("bub_date:  %02d \n", rec.bat.date);
    printf("bub_month: %02d \n", rec.bat.month);
    printf("bub_year:  %04d \n", rec.bat.years);


    printf("mb_cur[0]: %4.1f \n", rec.mb[0].cur*0.1);
    printf("mb_cur[1]: %4.1f \n", rec.mb[1].cur*0.1);

    printf("sma_tempA[0][0]: %4.1f \n", rec.sma[0][0].tempA*1.0);
    printf("sma_tempA[0][12]: %4.1f \n", rec.sma[0][12].tempA*1.0);
    printf("sma_tempA[0][13]: %4.1f \n", rec.sma[0][13].tempA*1.0);
    printf("sma_volt [0][13]: %2.1f \n", rec.sma[0][13].volt*0.001);
    
      
    printf("cell_volt[0][0][0]: %4.3f \n", rec.cell[0][0][0].volt*0.001);    
    printf("cell_volt[0][5][4]: %4.3f \n", rec.cell[0][5][4].volt*0.001);        
    printf("cell_temp[0][5][4]: %4.0f \n", rec.cell[0][5][4].temp*1.0);    
*/
    
    fclose(ofile);
}



