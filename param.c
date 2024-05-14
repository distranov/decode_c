#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "param.h"
//-----------------------------------------------------------------------------
param_struct param_table[] = {
    {"bub_date",            "%1.0f",  PARAM_TYPE_UINT8,  offsetof(rec_struct, bat.date),             1},
    {"bub_month",           "%1.0f",  PARAM_TYPE_UINT8,  offsetof(rec_struct, bat.month),            1},
    {"bub_years",           "%1.0f",  PARAM_TYPE_UINT16, offsetof(rec_struct, bat.years),            1},
    {"bub_hours",           "%1.0f",  PARAM_TYPE_UINT8,  offsetof(rec_struct, bat.hours),            1},
    {"bub_minutes",         "%1.0f",  PARAM_TYPE_UINT8,  offsetof(rec_struct, bat.minutes),          1},
    {"bub_seconds",         "%1.0f",  PARAM_TYPE_UINT8,  offsetof(rec_struct, bat.seconds),          1},
    {"bub_dm1",             "%1.0f",  PARAM_TYPE_UINT32, offsetof(rec_struct, bat.dm1_spn),          1},

    {"mb",                       "",  PARAM_TYPE_MB,     offsetof(rec_struct, mb),                   0},
    {"mb_cur",              "%1.1f",  PARAM_TYPE_INT16,  offsetof(mb_struct,  cur),                0.1},
    {"mb_volt",             "%1.1f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  volt),               0.1},
    {"mb_volt_bus",         "%1.1f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  volt_bus),           0.1},
    {"mb_charger_volt_lim", "%1.1f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  charger_volt_lim),   0.1},
    {"mb_volt_ign",         "%1.1f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  volt_ign),           0.1},
    {"mb_soc",              "%1.1f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  soc),                0.1},
    {"mb_cell_volt_min",    "%1.3f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  cell_volt_min),    0.001},    
    {"mb_cell_volt_max",    "%1.3f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  cell_volt_max),    0.001},
    {"mb_cell_volt_avg",    "%1.3f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  cell_volt_avg),    0.001},
    {"mb_cell_temp_min",    "%1.1f",  PARAM_TYPE_INT16,  offsetof(mb_struct,  cell_temp_min),      0.1},
    {"mb_cell_temp_max",    "%1.1f",  PARAM_TYPE_INT16,  offsetof(mb_struct,  cell_temp_max),      0.1},
    {"mb_cell_temp_avg",    "%1.1f",  PARAM_TYPE_INT16,  offsetof(mb_struct,  cell_temp_avg),      0.1},
    {"mb_cur_lim_dischrg",  "%1.1f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  cur_lim_dischrg),    0.1},
    {"mb_cur_lim_chrg",     "%1.1f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  cur_lim_chrg),       0.1},
    {"mb_temp_cool_intake", "%1.0f",  PARAM_TYPE_INT8,   offsetof(mb_struct,  temp_cool_intake),     1},
    {"mb_temp_cool_outlet", "%1.0f",  PARAM_TYPE_INT8,   offsetof(mb_struct,  temp_cool_outlet),     1},
    {"mb_temp_air_amb",     "%1.0f",  PARAM_TYPE_INT8,   offsetof(mb_struct,  temp_air_amb),         1},
    {"mb_temp_air_int",     "%1.0f",  PARAM_TYPE_INT8,   offsetof(mb_struct,  temp_air_int),         1},    
    {"mb_on",               "%1.0f",  PARAM_TYPE_INT16,  offsetof(mb_struct,  on),                   1},    
    {"mb_moto",             "%1.0f",  PARAM_TYPE_INT16,  offsetof(mb_struct,  moto),                 1},    
    {"mb_autonom_run",      "%1.0f",  PARAM_TYPE_INT16,  offsetof(mb_struct,  autonom_run),          1},    
    {"mb_total_run",        "%1.0f",  PARAM_TYPE_INT16,  offsetof(mb_struct,  total_run),            1},    
    {"mb_num_of_charges",   "%1.0f",  PARAM_TYPE_INT16,  offsetof(mb_struct,  num_of_charges),       1},    
    {"mb_speed",            "%1.0f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  speed),                1},
    {"mb_pki_iso",          "%1.0f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  pki_iso),              1},
    {"mb_pki_volt",         "%1.1f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  pki_volt),           0.1},
    {"mb_soc_kalman",       "%1.1f",  PARAM_TYPE_UINT16, offsetof(mb_struct,  soc_kalman),         0.1},

    {"sma",                      "",  PARAM_TYPE_SMA,    offsetof(rec_struct, sma),                 0},
    {"sma_tempA",           "%1.0f",  PARAM_TYPE_INT8,   offsetof(sma_struct, tempA),               1},
    {"sma_tempB",           "%1.0f",  PARAM_TYPE_INT8,   offsetof(sma_struct, tempA),               1},
    {"sma_volt",            "%1.3f",  PARAM_TYPE_UINT16, offsetof(sma_struct, volt),            0.001},

    {"cell",                    "",  PARAM_TYPE_CELL,   offsetof(rec_struct, cell),                 0},
    {"cell_volt",          "%1.3f",  PARAM_TYPE_UINT16, offsetof(cell_struct, volt),            0.001},
    {"cell_temp",          "%1.0f",  PARAM_TYPE_INT8,   offsetof(cell_struct, temp),                1},
};

static param_struct param[PARAM_LIST_LENGTH] = {0}; // параметры для выдачи
static uint32_t param_num = 0;                      // число параметров
//-----------------------------------------------------------------------------
// смещение элемента масссива относительно начала массива
uint32_t param_offset_from_begin(uint32_t size, uint32_t mb, uint32_t sma, uint32_t cell) {
    const uint32_t max_cell = 12;
    const uint32_t max_sma  = 14;
    return(mb*max_sma*max_cell + sma*max_cell*size + cell*size);
}
//-----------------------------------------------------------------------------
void param_cut_string(char *str, char sym) {
    for (uint32_t i = 0; i < strlen(str); i++) {
        if (str[i] == sym) str[i] = 0;
    }
}
//-----------------------------------------------------------------------------
int32_t param_find_index(const char *param, char sym) {
    char str[100];
    strcpy(str, param);
    param_cut_string(str, sym);
    for (uint32_t i = 0; i < sizeof(param_table)/sizeof(param_struct); i++)
        if(strcmp(str, param_table[i].param) == 0)
            return i;
    return -1;
}
//-----------------------------------------------------------------------------
bool param_is_ready(char *str) {
    while(*str == ' ') str++; // 
    if (*str == '#') return false;  
    if (*str == '\n') return false;   
    param_cut_string(str, ' ');
    return true;
}
//-----------------------------------------------------------------------------
// в зависимости от числа аргументов
bool param_is_array(char *str, uint16_t dim[3]) {
    uint32_t c_num = 0;
    uint32_t c[3];
    for(uint32_t i = 0; i < strlen(str); i++) {
        if (str[i] == '[') c[c_num++] = atoi(&str[i+1]);
    }    
    switch(c_num) {
        case 1 : dim[2] = c[0]; break;
        case 2 : dim[2] = c[1]; dim[1] = c[0]; break;
        case 3 : dim[2] = c[2]; dim[1] = c[1]; dim[0] = c[0]; break;
    }
    return (c_num > 0) ? true : false;
}
//-----------------------------------------------------------------------------
uint8_t param_init(const char *filename) {
    FILE *file = fopen("list_param.cfg", "r");
    if(file == NULL) {
        printf("file list_param.cfg not found \n");
        return 1;
    }
    // read list_param.cfg     
    char *line = NULL;
    char *str = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1) {        
        str = line;
        if(!param_is_ready(str)) continue;

        // копируем данные параметра из таблицы
        int32_t index = param_find_index(str, '[');
        if(index == -1) continue;
        memcpy(&param[param_num], &param_table[index], sizeof(param_struct));
        // меняем имя параметра
        strcpy(param[param_num].param, str);
        // если массив то рассчитываем новое значение смещения
        uint16_t dim[3] = {0};
        if(param_is_array(str, dim)) {
            index = param_find_index(str, '_');
            if(index == -1) continue;
            uint32_t array_offset = param_table[index].start_byte; 
            // определяем смещение элемента массива
            uint32_t struct_offset;
            switch(param_table[index].type) {
                case PARAM_TYPE_MB   : struct_offset = param_offset_from_begin(sizeof(mb_struct),   dim[0], dim[1], dim[2]); break;
                case PARAM_TYPE_SMA  : struct_offset = param_offset_from_begin(sizeof(sma_struct),  dim[0], dim[1], dim[2]); break;
                case PARAM_TYPE_CELL : struct_offset = param_offset_from_begin(sizeof(cell_struct), dim[0], dim[1], dim[2]); break;
                default : break;  
            }
            param[param_num].start_byte += array_offset + struct_offset;
        }
        param_num++;  
        if(param_num >= PARAM_LIST_LENGTH) break;
    }
    free(line);
    fclose(file);

    if(param_num == 0) return 2;
    return 0;
}
//-----------------------------------------------------------------------------
char *param_data2str(rec_struct *rec) {    
    bool first_dig = true;
    static char str[1000];
    char dig[10];
    float res = 0;
    uint64_t pdata;
    sprintf(str, " ");    
    for (uint32_t i = 0; i < param_num; i++) {
        pdata = (uint64_t)rec + param[i].start_byte;
        switch(param[i].type) {
            case PARAM_TYPE_UINT8  : res = *((uint8_t*)pdata)  * param[i].factor; break;
            case PARAM_TYPE_INT8   : res = *((int8_t*)pdata)   * param[i].factor; break;
            case PARAM_TYPE_UINT16 : res = *((uint16_t*)pdata) * param[i].factor; break;
            case PARAM_TYPE_INT16  : res = *((int16_t*)pdata)  * param[i].factor; break;
            case PARAM_TYPE_UINT32 : res = *((uint32_t*)pdata) * param[i].factor; break;
            case PARAM_TYPE_INT32  : res = *((int32_t*)pdata)  * param[i].factor; break;
            default : break;
        }        
        if(!first_dig) 
            strcat(str, ",");
        else
            first_dig = false;
        sprintf(dig, param[i].sconv, res);
        strcat(str, dig);
    }
    strcat(str, "\n");
    return str;
}
//-----------------------------------------------------------------------------
char *param_header(void) {
    bool first_dig = true;
    static char str[1000];
    sprintf(str, " ");
    for (uint32_t i = 0; i < param_num; i++) {
        if(!first_dig) 
            strcat(str, ",");
        else
            first_dig = false;
        strcat(str, param[i].param);                       
    }
    strcat(str, "\r\n");
    return str;
}
