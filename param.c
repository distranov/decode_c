#include <stdbool.h>
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
    {"bub_dm1",        "%06.0f", PARAM_TYPE_UINT16, offsetof(rec_struct, bat.dm1_spn), 0,   1, 0},


    {"mb",                  "",  PARAM_TYPE_MB,     offsetof(rec_struct, mb),          0, 0.1, 0},
    {"mb_cur",         "%3.1f",  PARAM_TYPE_INT16,  offsetof(mb_struct, cur),          0, 0.1, 0},
    {"mb_volt",        "%3.1f",  PARAM_TYPE_UINT16, offsetof(mb_struct, volt),         0, 0.1, 0},
    {"mb_volt_bus",    "%3.1f",  PARAM_TYPE_UINT16, offsetof(mb_struct, volt_bus),     0, 0.1, 0},


mb_charger_volt_lim[0]             # максимальное напряжение зарядной станции V
mb_volt_ign[0]                     # напряжение низковольтной сети V
mb_soc[0]                          # soc состояние заряда
mb_cell_volt_min[0]                # минимальное напряжение ячеек mV
mb_cell_volt_max[0]                # максимальное напряжение ячеек mV
mb_cell_volt_avg[0]                # среднее напряжение ячеек mV
mb_cell_temp_min[0]                # максимальная температура ячеек °C
mb_cell_temp_max[0]                # минимальная температура ячеек °C
mb_cell_temp_avg[0]                # средняя температура ячеек  °C
mb_cur_lim_dischrg[0]              # ограничение тока разряда A
mb_cur_lim_chrg[0]                 # ограничение тока заряда  A
mb_temp_cool_intake[0]             # температура охлаждающей жидкости на входе СНЭ °C
mb_temp_cool_outlet[0]             # температура охлаждающей жидкости на выходе СНЭ °C
mb_temp_air_amb[0]                 # температура воздуха окружающей среды °C
mb_temp_air_int[0]                 # температура воздуха внутри модуля °C



    {"sma",                 "",  PARAM_TYPE_SMA,    offsetof(rec_struct, sma),         0, 0, 0},
    {"sma_tempA",      "%2.1f",  PARAM_TYPE_INT8,   offsetof(sma_struct, tempA),       0, 1, 0},
    {"sma_volt",       "%2.1f",  PARAM_TYPE_UINT16, offsetof(sma_struct, volt),        0, 0.001, 0},
    

    {"cell",                "",  PARAM_TYPE_CELL,   offsetof(rec_struct, cell),        0, 0, 0},
    {"cell_volt",      "%4.3f",  PARAM_TYPE_INT16,  offsetof(cell_struct, volt),       0, 0.001, 0},
    {"cell_temp",      "%2.0f",  PARAM_TYPE_INT8,   offsetof(cell_struct, temp),       0,   1, 0},


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
            printf("---- c1:%d c2:%d c3:%d \n", dim[0], dim[1], dim[2]);
            // определяем смещение массива         
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

    printf("par_num: %03d \n", param_num);
    for (uint32_t i = 0; i < param_num; i++) {
        printf("select: %s arg: %s start_byte: %04d \n", param[i].param, param[i].sconv, param[i].start_byte);        
    }
}
//-----------------------------------------------------------------------------
char *param_data2str(rec_struct *rec) {    
    bool first_dig = true;
    static char str[1000];

    char dig[10];
    float res = 0;
    sprintf(str, "");

    uint64_t pdata;
    for (uint32_t i = 0; i < param_num; i++) {
        pdata = (uint64_t)rec + param[i].start_byte;
        switch(param[i].type) {
            case PARAM_TYPE_UINT8  : res = *((uint8_t*)pdata)  * param[i].factor; break;
            case PARAM_TYPE_INT8   : res = *((int8_t*)pdata)   * param[i].factor; break;
            case PARAM_TYPE_UINT16 : res = *((uint16_t*)pdata) * param[i].factor; break;
            case PARAM_TYPE_INT16  : res = *((int16_t*)pdata)  * param[i].factor; break;
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
