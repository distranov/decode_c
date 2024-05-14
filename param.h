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
    PARAM_TYPE_ENUM     = 6,
    PARAM_TYPE_MB       = 7,
    PARAM_TYPE_SMA      = 8,
    PARAM_TYPE_CELL     = 9,
    PARAM_TYPE_INT32    = 10,
    PARAM_TYPE_UINT32   = 11,
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
    double           factor;
} param_struct;

uint8_t param_init(const char *filename);
char *param_data2str(rec_struct *rec);
char *param_header(void);

#endif