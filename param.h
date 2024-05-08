#ifndef _PARAM_H_
#define _PARAM_H_

#include <stdint.h>

#include "file2data.h"

#define PARAM_LIST_LENGTH 100

typedef enum {
    PARAM_TYPE_NONE     = 0,
    PARAM_TYPE_INT16    = 1,
    PARAM_TYPE_UINT16   = 2,
    PARAM_TYPE_INT8     = 3,
    PARAM_TYPE_UINT8    = 4,
    PARAM_TYPE_DATETIME = 5,
    PARAM_TYPE_ENUM     = 6
} param_type_enum;

typedef struct {
    uint16_t num;
    int8_t  mb;
    int8_t  sma;
    int8_t  cell;
} param_list_struct;

typedef struct {
    char             param[40];
    char             sconv[32];                       
    param_type_enum  type;
    uint32_t         start_byte;
    uint32_t         start_bit;
    double           factor;
    double           offset;
} param_struct;




void param_init(const char *filename);
void param_init_new(const char *filename);


char *param_data2str(rec_struct *rec);

void param_cut_string(char *str, char sym);

#endif