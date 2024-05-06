#include <stddef.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "struct.h"
#include "crc32.h"


typedef enum
{
 PARAM_TYPE_NONE     = 0,
 PARAM_TYPE_INT16    = 1,
 PARAM_TYPE_UINT16   = 2,
 PARAM_TYPE_INT8     = 3,
 PARAM_TYPE_UINT8    = 4,
 PARAM_TYPE_DATETIME = 5,
 PARAM_TYPE_ENUM     = 6
} param_type_enum;

typedef struct {
    char             param[40];
    char             sconv[32];                       
    param_type_enum  type;
    uint32_t         start_byte;
    uint32_t         start_bit;
    double           factor;
    double           offset;
} param_table_struct;


param_table_struct param_table[] = {
    {"date",       "%02.0f", PARAM_TYPE_UINT8,  offsetof(sdcard_packet100_struct, bat.date),    0,   1, 0},
    {"month",      "%02.0f", PARAM_TYPE_UINT8,  offsetof(sdcard_packet100_struct, bat.month),   0,   1, 0},
    {"years",      "%02.0f", PARAM_TYPE_UINT16, offsetof(sdcard_packet100_struct, bat.years),   0,   1, 0},
    {"hours",      "%02.0f", PARAM_TYPE_UINT8,  offsetof(sdcard_packet100_struct, bat.hours),   0,   1, 0},
    {"minutes",    "%02.0f", PARAM_TYPE_UINT8,  offsetof(sdcard_packet100_struct, bat.minutes), 0,   1, 0},
    {"seconds",    "%02.0f", PARAM_TYPE_UINT8,  offsetof(sdcard_packet100_struct, bat.seconds), 0,   1, 0},

    {"string_cur", "%4.1f",PARAM_TYPE_INT16,  offsetof(sdcard_packet100_struct, mb.cur),      0, 0.1, 0},

};

uint32_t param_list[100];
uint32_t param_cnt = 0;

char *Data2Str(sdcard_packet100_struct *data, uint16_t param) {
    
    static char str[40];
    float res;
    uint8_t *pdata = (uint8_t*)data + param_table[param].start_byte;


    switch(param_table[param].type) {

        case PARAM_TYPE_UINT8  : res =  *((uint8_t*) pdata) * param_table[param].factor; break;
        case PARAM_TYPE_INT8   : res =  *((int8_t*) pdata)  * param_table[param].factor; break;


        case PARAM_TYPE_UINT16 : res = *((uint16_t*)pdata) * param_table[param].factor; break;
        case PARAM_TYPE_INT16  : res = *((int16_t*)pdata)  * param_table[param].factor; break;


    }
    sprintf(str, param_table[param].sconv, res);
    return str;
}


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

    FILE *ifile = fopen(argv[1], "r");
    if(ifile == NULL) {
        printf("file not found\n");
        return 3;
    }

// conversion filename to *.csv
    *strstr(argv[1], ".CXE") = 0;
    strcat(argv[1], ".csv");
 
    FILE *ofile = fopen(argv[1], "w");
    if(ofile == NULL) {
        printf("failed to create file\n");
        return 4;
    }

// load list_param.cfg
    FILE *lfile = fopen("list_param.cfg", "r");
    if(lfile == NULL) {
        printf("file list_param.cfg not found\n");
        return 5;
    }

// read list_param.cfg     
    char *line = NULL;
    char *str = NULL;
    size_t len = 0;
    size_t read;
    uint16_t line_cnt = 0;
    while ((read = getline(&line, &len, lfile)) != -1) {
        line_cnt++;
        str = line;
        while(*str == ' ') str++;
        if (*str == '#') continue;
        for(uint32_t i = 0; i < strlen(str); i++)   
            if(*(str + i) == ' ') *(str + i) = 0;

        for (uint32_t i = 0; i < sizeof(param_table)/sizeof(param_table_struct); i++) {
            if(strncmp(str, param_table[i].param, strlen(str)) == 0) {
                param_list[param_cnt++] = i;
                printf("line_cnt: %02d str: %s \n", line_cnt, line);
            }
            
/*
            if(str != NULL) { 
                printf("1: %s ", str);
                printf("2: %s \n", param_table[i].param);
                printf("cmp: %02d", strncmp(str, param_table[i].param, strlen(param_table[i].param)));
                //printf("select: %s \n", param_table[i].param);
                    param_list[param_cnt++] = i;
            }*/
        }
        

        //printf("%s", line);
    }
    printf("par_cnt: %03d \n", param_cnt);
    for (uint32_t i = 0; i < param_cnt; i++) {
        printf("select: %s \n", param_table[param_list[i]].param);        
    }

    sdcard_packet100_struct data = {0}; 
    uint32_t cnt = 0;
    //fread (&head, sizeof(sdcard_packet_info_struct), 1, in_files[i]);
    while (fread((void*)&data, sizeof(sdcard_packet100_struct), 1, ifile))
        if (CRC32_Soft((uint32_t*) &data, sizeof(sdcard_packet100_struct)/sizeof(uint32_t)) == 0) {
            cnt++;
            if(cnt > 15) break;
            for(uint32_t i = 0; i < param_cnt; i++) {
                printf("%s ", Data2Str(&data, param_list[i]));
            }
            printf(" \n");
        }
            


    printf("cnt : %d \n", cnt);

    fclose(ifile);
    fclose(ofile);
}



