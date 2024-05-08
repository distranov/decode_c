#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "param.h"
//-----------------------------------------------------------------------------
param_struct param_table[] = {
    {"bub_date",       "%02.0f", PARAM_TYPE_UINT8,  offsetof(rec_struct, bat.date),    0,   1, 0},
    {"bub_month",      "%02.0f", PARAM_TYPE_UINT8,  offsetof(rec_struct, bat.month),   0,   1, 0},
    {"bub_years",      "%02.0f", PARAM_TYPE_UINT16, offsetof(rec_struct, bat.years),   0,   1, 0},
    {"bub_hours",      "%02.0f", PARAM_TYPE_UINT8,  offsetof(rec_struct, bat.hours),   0,   1, 0},
    {"bub_minutes",    "%02.0f", PARAM_TYPE_UINT8,  offsetof(rec_struct, bat.minutes), 0,   1, 0},
    {"bub_seconds",    "%02.0f", PARAM_TYPE_UINT8,  offsetof(rec_struct, bat.seconds), 0,   1, 0},

    {"mb",             "%4.1f",  PARAM_TYPE_INT16,  offsetof(rec_struct, mb),          sizeof(mb_struct), 0.1, 0},
    {"mb_cur",         "%4.1f",  PARAM_TYPE_INT16,  offsetof(mb_struct, cur),          0, 0.1, 0},

    {"sma",            "%4.1f",  PARAM_TYPE_INT16,  offsetof(rec_struct, sma),         sizeof(sma_struct), 0.1, 0},
    {"sma_tempA",      "%4.1f",  PARAM_TYPE_INT16,  offsetof(sma_struct, tempA),       0, 0.1, 0},

    {"cell",           "%4.1f",  PARAM_TYPE_INT16,  offsetof(rec_struct, cell),        sizeof(cell_struct), 0.1, 0},
    {"cell_volt",      "%4.1f",  PARAM_TYPE_INT16,  offsetof(cell_struct, volt),       0, 0.1, 0},


};

uint32_t offset(uint32_t size, uint32_t mb, uint32_t sma, uint32_t cell) {
    const uint32_t max_cell = 12;
    const uint32_t max_sma  = 14;
    //const uint32_t max_mb   = 2;
    return(mb*max_sma*max_cell + sma*max_cell*size + cell*size);
}


static param_list_struct param_list[PARAM_LIST_LENGTH] = {0};
static uint32_t param_cnt = 0;



//-----------------------------------------------------------------------------
// При обработке файла "list_param.cfg" формировать таблицу param_table динамически?
// При обработке файла "list_param.cfg" формировать смещение byte динамически?
//-----------------------------------------------------------------------------
void param_init(const char *filename) {
    FILE *file = fopen("list_param.cfg", "r");
    if(file == NULL) {
        printf("file list_param.cfg not found \n");
        return;
    }

    // read list_param.cfg     
    char *line = NULL;
    char *str = NULL;
    size_t len = 0;

    while (getline(&line, &len, file) != -1) {
        
        str = line;
        while(*str == ' ') str++;
        if (*str == '#') continue;
        param_cut_string(str, ' ');

        for (uint32_t i = 0; i < sizeof(param_table)/sizeof(param_struct); i++) {
            if(strncmp(str, param_table[i].param, strlen(str)) == 0) {
                param_list[param_cnt].mb = -1;
                param_list[param_cnt].sma = -1;
                param_list[param_cnt].cell = -1;                
                param_list[param_cnt++].num = i;
            }
        }        
    }
    free(line);
    fclose(file);

    param_list[param_cnt].num = 11;
    param_list[param_cnt].mb = 0;
    param_list[param_cnt].sma = 13;
    param_list[param_cnt++].cell = 9;

    printf("par_cnt: %03d \n", param_cnt);
    for (uint32_t i = 0; i < param_cnt; i++) {
        printf("select: %s [%02d][%02d][%02d] \n", param_table[param_list[i].num].param, param_list[i].mb, param_list[i].sma, param_list[i].cell);        
    }
}
//-----------------------------------------------------------------------------
char *param_data2str(rec_struct *rec) {
    
    static char str[40];
    float res = 0;

    param_list_struct param;
    param = param_list[6];
    printf("%d_%d_%d_%d \n", param.num, param.mb, param.sma, param.cell);


    if((param.mb == -1) && (param.sma == -1) && (param.cell == -1)) {
        uint8_t *pdata = (uint8_t*)rec + param_table[param.num].start_byte;

        switch(param_table[param.num].type) {
            case PARAM_TYPE_UINT8  : res = *((uint8_t*)pdata)  * param_table[param.num].factor; break;
            case PARAM_TYPE_INT8   : res = *((int8_t*)pdata)   * param_table[param.num].factor; break;
            case PARAM_TYPE_UINT16 : res = *((uint16_t*)pdata) * param_table[param.num].factor; break;
            case PARAM_TYPE_INT16  : res = *((int16_t*)pdata)  * param_table[param.num].factor; break;
            default : break;
        }
    }
    else {
        char str[40];                
        strcpy(str, param_table[param.num].param);
        printf("%s \n", str);
        /*
        for(int i = 0; i < strlen(str); i++) {
            if(str[i] == '_') str[i] = 0;
        }
        */
        param_cut_string(str, '_'); 
        printf("%s \n", str);
        



    }





 // sprintf(str, param_table[param.num].sconv, res);
    return str;
}

//-----------------------------------------------------------------------------
void param_cut_string(char *str, char sym) {
    for (uint32_t i = 0; i < strlen(str); i++) {
        if (str[i] == sym) str[i] = 0;
    }

}
//-----------------------------------------------------------------------------
int32_t param_index(const char *param) {
    for (uint32_t i = 0; i < sizeof(param_table)/sizeof(param_struct); i++)
        if(strncmp(param, param_table[i].param, strlen(param)) == 0)
            return i;
}

//-----------------------------------------------------------------------------
static param_struct param[PARAM_LIST_LENGTH] = {0};
static uint32_t param_num = 0;

void param_init_new(const char *filename) {
    FILE *file = fopen("list_param.cfg", "r");
    if(file == NULL) {
        printf("file list_param.cfg not found \n");
        return;
    }

    // read list_param.cfg     
    char *line = NULL;
    char *str = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1) {        
        str = line;
        while(*str == ' ') str++; // 
        if (*str == '#') continue;  
        if (*str == '\n') continue;   
        param_cut_string(str, ' ');
        //if(*str == 0) continue;
        printf("find.. %s \n", str);

        // определяем индексы и их количество        
        uint32_t c[3] = {0};
        uint32_t c_num = 0;
        for(uint32_t i = 0; i < strlen(str); i++) {
            if (str[i] == '[') c[c_num++] = atoi(&str[i+1]);
        }
        printf("num:%d c1:%d c2:%d c3:%d \n", c_num, c[0], c[1], c[2]);
        // если число индексов не нулевое определяем начало массива
        // выделяем основную структуру до '_'
        char substr[10] = {0};
        for(uint32_t i = 0; i < 10; i++)
            if(str[i] != '_')
                substr[i] = str[i];
            else 
                break;
        int32_t index = param_index(substr);
        uint32_t array_offset = param_table[index].offset;  // определяем смщение массива

        param_cut_string(str, '[');
        index = param_index(str);
        uint32_t struct_offset = param_table[index].offset; // определяем смещение структуры


        printf("substring: %s offset arr: %d, offset struct: %d \n", substr, array_offset, struct_offset);
        
        
        

        // определяем адрес элемента


        /*
        for (uint32_t i = 0; i < sizeof(param_table)/sizeof(param_struct); i++) {
            if(strncmp(str, param_table[i].param, strlen(str)) == 0) {
                param_list[param_cnt].mb = -1;
                param_list[param_cnt].sma = -1;
                param_list[param_cnt].cell = -1;                
                param_list[param_cnt++].num = i;
            }
        } */       
    }
    free(line);
    fclose(file);

    param_list[param_cnt].num = 11;
    param_list[param_cnt].mb = 0;
    param_list[param_cnt].sma = 13;
    param_list[param_cnt++].cell = 9;

    printf("par_cnt: %03d \n", param_cnt);
    for (uint32_t i = 0; i < param_cnt; i++) {
        printf("select: %s [%02d][%02d][%02d] \n", param_table[param_list[i].num].param, param_list[i].mb, param_list[i].sma, param_list[i].cell);        
    }
}