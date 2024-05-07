#ifndef _FILE2DATA_H_
#define _FILE2DATA_H_

#include <stdint.h>

typedef enum {
    F2D_PACKET_ERROR        = 0,
    F2D_PACKET_VERSION_100  = 1,
    F2D_PACKET_VERSION_101  = 2,
    F2D_PACKET_VERSION_102  = 3,
    F2D_PACKET_VERSION_1000 = 4,
    F2D_PACKET_FILE_ERROR   = 5,
    
} f2d_packet_version_enum;

typedef enum {
    F2D_REC_OK   = 0,
    F2D_REC_END  = 1,
} f2d_status;



typedef struct {
    uint8_t  dev_type;
    uint8_t  bat_type;
    uint8_t  ev_type;
    uint8_t  j1939_adr;                   // адрес j1939 модуля
    uint32_t rkin;
    uint32_t rkout;
    uint32_t data[4];                     // данные для хранения в eeprom
    // текущие дата и время
    uint8_t  date;
    uint8_t  month;
    uint16_t years;
    uint8_t  hours;
    uint8_t  minutes;
    uint8_t  seconds;
    uint8_t  subseconds;
    // данные sdcard
    uint16_t sdcard_free;                 // свободное пространство SDCARD в MB
    uint16_t sdcard_total;                // общий размер SDCARD в MB
    // отказы dm1
    uint32_t dm1_spn;                     //
} __attribute__ ((__packed__)) bat_struct;

typedef struct {
    // Data1
    int16_t  cur;                         // 0.1A ток
    uint16_t volt;                        // 0.1V напряжение
    // Data2
    uint16_t soc;                         // 0.1%
    uint16_t cell_volt_min;               // 0.001V максимальное напряжение ячеек
    uint16_t cell_volt_max;               // 0.001V минимальное напряжение ячеек
    // Data3
    int16_t  cell_temp_min;               // 0.1°C максимальная температура ячеек
    int16_t  cell_temp_max;               // 0.1°C минимальная температура ячеек
    int16_t  cell_temp_avg;               // 0.1°C средняя температура ячеек
    // Data4
    uint16_t charge_bat;                  // 0.01Аh текущий заряд батареи
    uint16_t cell_bal_cnt;                // число ячеек участвующих в балансировке
    // Data5
    uint16_t cur_lim_dischrg;             // 0.1 A ограничение тока разряда
    uint16_t cur_lim_chrg;                // 0.1 A ограничение тока заряда
    // Data6
    uint16_t volt_bus;                    // 0.1 V напряжение высоковольтной шины со стороны ТС
    uint16_t volt_ign;                    // 0.1 V напряжения низковольтного питания со стороны ТС
    int8_t  temp_cool_intake;             // 0.1  °C температура охлаждающей жидкости на входе СНЭ
    int8_t  temp_cool_outlet;             // 0.1  °C температура охлаждающей жидкости на выходе СНЭ
    int8_t  temp_air_amb;                 // 0.1  °C температура воздуха окружающей среды
    int8_t  temp_air_int;                 // 0.1  °C температура воздуха внутри модуля
    // Data7
    uint16_t charger_volt_lim;            // 0.1 V максимальное напряжение зарядной станции
    // Data8
    uint8_t  cell_volt_max_mod_num;       // 1 номер модуля где расположена ячейка с максимальным напряжением
    uint8_t  cell_volt_max_num;           // 1 номер ячейки с максимальным напряжением в указанном модуле
    uint8_t  cell_volt_min_mod_num;       // 1 номер модуля где расположена ячейка с минимальным напряжением
    uint8_t  cell_volt_min_num;           // 1 номер ячейки с минимальным напряжением в указанном модуле
    uint16_t cell_volt_avg;               // 0.001V среднее напряжение ячеек
    // Data9
    uint8_t  cell_temp_max_mod_num;       // 1 номер модуля где расположена ячейка с максимальной температурой
    uint8_t  cell_temp_max_num;           // 1 номер ячейки с максимальной температурой в указанном модуле
    uint8_t  cell_temp_min_mod_num;       // 1 номер модуля где расположена ячейка с минимальной температурой
    uint8_t  cell_temp_min_num;           // 1 номер ячейки с минимальной температурой в указанном модуле
    // Data10
    // Data11
    // Status1
    struct {
        uint64_t bus_pos_con_st       : 2;    // BF состояние положительного HV контактора
        uint64_t bus_neg_con_st       : 2;    // BF состояние отрицательного HV контактора
        uint64_t bus_disc_forewar     : 4;    // BF подготовка к отключению от шины и причина
        uint64_t bus_prechrg_relay_st : 2;    // BF состояние реле предзаряда шины
        uint64_t center_con_st        : 2;    // BF состояние центрального контактора
        uint64_t bus_actv_isol_st     : 4;    // BF состояние активного теста изоляции
        uint64_t bus_pasv_isol_st     : 4;    // BF состояние пассивного теста изоляции
        uint64_t hvil_st              : 2;    // BF состояние контура блокировки HVIL
        uint64_t acc_st               : 2;    // BF состояние датчика инерции(акселерометра)
        uint64_t soc_st               : 4;    // BF состояние soc
        uint64_t cell_bal_st          : 2;    // BF "сбалансированность" системы
        uint64_t cell_bal_actv        : 2;    // BF состояние процесса балансировки
        uint64_t int_chrgr_st         : 4;    // BF состояние внутреннего зарядного устройства
        uint64_t hves_st_counter      : 4;    // BF контрольный таймер "зависания"
        uint64_t bus_conn_st          : 4;    // BF состояние подключения СНЭ к шине
        uint64_t hves_oper_st         : 4;    // BF режим работы СНЭ
        uint64_t num_of_esp_rdy       : 8;    // 1 число параллельно подключенных МБ к шине
        uint64_t hves_st_crc          : 8;    // 1 контрольная сумма
    } status1;
    // Status2
    struct {
        uint64_t dpl_soc      : 2;            // BF ограничение мощности разряда из-за soc
        uint64_t dpl_temp     : 2;            // BF ограничение мощности разряда из-за температуры
        uint64_t dpl_bat_diag : 2;            // BF ограничение мощности разряда из-за диагностики батареи
        uint64_t dpl_volt     : 2;            // BF ограничение мощности разряда из-за напряжения СНЭ или отдельной ячейки
        uint64_t dpl_cur      : 2;            // BF ограничение мощности разряда из-за тока батареи
        uint64_t dpl_undef    : 2;            // BF ограничение мощности разряда из-за неопределенной причины
        uint64_t dpl_res      : 20;           // BF резерв
        uint64_t cpl_soc      : 2;            // BF ограничение мощности заряда из-за soc
        uint64_t cpl_temp     : 2;            // BF ограничение мощности заряда из-за температуры
        uint64_t cpl_bat_diag : 2;            // BF ограничение мощности заряда из-за диагностики батареи
        uint64_t cpl_volt     : 2;            // BF ограничение мощности заряда из-за напряжения СНЭ или отдельной ячейки
        uint64_t cpl_cur      : 2;            // BF ограничение мощности заряда из-за тока батареи
        uint64_t cpl_undef    : 2;            // BF ограничение мощности заряда из-за неопределенной причины
        uint64_t cpl_res      : 20;           // BF резерв
    } status2;
    // pgn = 65502
    uint32_t intfault;                    // BF слово отказов внутренних
    uint32_t extfault;                    // BF слово отказов внешних
    // pgn = 65503
    uint16_t on;                          // 1   число включений
    uint16_t moto;                        // час время наработки
    uint16_t autonom_run;                 // км  пробег в режиме автономного хода
    uint16_t total_run;                   // км  общий пробег
    // pgn = 65504
    uint16_t num_of_charges;              // 1  число зарядок
    uint16_t energy_in;                   // Ah принятая энергия
    uint16_t energy_out;                  // Ah выданная энергия
    uint16_t energy_heat;                 // Ah выданная энергия
    // pgn = 65505
    uint16_t num_of_fault_disc;           // число аварийных отключений
    uint16_t speed;                       // км/ч
    // pgn = 65506
    uint32_t state_in;
    uint32_t state_out;
    // pki
    uint8_t  pki_cnt;                  // life cnt
    uint8_t  pki_state;                // state
    uint16_t pki_iso;                  // 
    uint16_t pki_volt;                 //
    uint16_t pki_pos;                  //

    uint16_t soc_kalman;
} __attribute__ ((__packed__)) mb_struct;

typedef struct {
    uint16_t fan_freq;                 // rpm  частота вращения вентилятора
    int8_t   tempA;                    // °C   температуры платы БУС
    int8_t   tempB;                    // °C   температуры платы БУС
    uint16_t volt;                     // 0.1V напряжение _ выходе СМА
    uint16_t fault;                    // BF   отказы СМА
} __attribute__ ((__packed__)) sma_struct;

typedef struct {
    uint16_t volt;                     // mV     напряжение ячейки
    uint16_t bal_cur;                  // mA     ток балансировки
    int8_t   temp;                     // °C     температура
    uint8_t  bal_cell_task;            // *10mAh задание на балансировку
} __attribute__ ((__packed__)) cell_struct;

typedef struct {
    uint16_t lv_pwr_in;                // 0.5 W/bit потребление TMS с низковольтной линии
    uint16_t hv_pwr_in;                // 0.5 W/bit потребление TMS с высоковольтной линии
    uint8_t  comp_speed;               // 32 rpm    скорость вращения компрессора
    uint8_t  rel_hmdt;                 // 0.4 %     относительная влажность
    uint16_t heater_st : 2;            // BF        состояние нагревателя
    uint16_t hvil_st   : 2;            // BF        состояние HVIL
    uint16_t mode      : 4;            // BF        режим работы
    uint16_t cool_lvl  : 2;            // BF        уровень ОЖ
    uint16_t res61705  : 6;            // BF
} __attribute__ ((__packed__)) tms_struct;

typedef struct {    
    bat_struct      bat;
    mb_struct       mb[2];
    sma_struct      sma[2][14];
    cell_struct     cell[2][14][12];
    tms_struct      tms[2];
} __attribute__ ((__packed__)) rec_struct;

typedef struct {
    // Data1
    int16_t  cur;                         // 0.1A ток
    uint16_t volt;                        // 0.1V напряжение
    // Data2
    uint16_t soc;                         // 0.1%
    uint16_t cell_volt_min;               // 0.001V максимальное напряжение ячеек
    uint16_t cell_volt_max;               // 0.001V минимальное напряжение ячеек
    // Data3
    int16_t  cell_temp_min;               // 0.1°C максимальная температура ячеек
    int16_t  cell_temp_max;               // 0.1°C минимальная температура ячеек
    int16_t  cell_temp_avg;               // 0.1°C средняя температура ячеек
    // Data4
    uint16_t charge_bat;                  // 0.01Аh текущий заряд батареи
    uint16_t cell_bal_cnt;                // число ячеек участвующих в балансировке
    // Data5
    uint16_t cur_lim_dischrg;             // 0.1 A ограничение тока разряда
    uint16_t cur_lim_chrg;                // 0.1 A ограничение тока заряда
    // Data6
    uint16_t volt_bus;                    // 0.1 V напряжение высоковольтной шины со стороны ТС
    uint16_t volt_ign;                    // 0.1 V напряжения низковольтного питания со стороны ТС
    int8_t  temp_cool_intake;             // 0.1  °C температура охлаждающей жидкости на входе СНЭ
    int8_t  temp_cool_outlet;             // 0.1  °C температура охлаждающей жидкости на выходе СНЭ
    int8_t  temp_air_amb;                 // 0.1  °C температура воздуха окружающей среды
    int8_t  temp_air_int;                 // 0.1  °C температура воздуха внутри модуля
    // Data7
    uint16_t charger_volt_lim;            // 0.1 V максимальное напряжение зарядной станции
    // Data8
    uint8_t  cell_volt_max_mod_num;       // 1 номер модуля где расположена ячейка с максимальным напряжением
    uint8_t  cell_volt_max_num;           // 1 номер ячейки с максимальным напряжением в указанном модуле
    uint8_t  cell_volt_min_mod_num;       // 1 номер модуля где расположена ячейка с минимальным напряжением
    uint8_t  cell_volt_min_num;           // 1 номер ячейки с минимальным напряжением в указанном модуле
    uint16_t cell_volt_avg;               // 0.001V среднее напряжение ячеек
    // Data9
    uint8_t  cell_temp_max_mod_num;       // 1 номер модуля где расположена ячейка с максимальной температурой
    uint8_t  cell_temp_max_num;           // 1 номер ячейки с максимальной температурой в указанном модуле
    uint8_t  cell_temp_min_mod_num;       // 1 номер модуля где расположена ячейка с минимальной температурой
    uint8_t  cell_temp_min_num;           // 1 номер ячейки с минимальной температурой в указанном модуле
    // Data10
    // Data11
    // Status1
    struct
    {
    uint64_t bus_pos_con_st       : 2;   // BF состояние положительного HV контактора
    uint64_t bus_neg_con_st       : 2;   // BF состояние отрицательного HV контактора
    uint64_t bus_disc_forewar     : 4;   // BF подготовка к отключению от шины и причина
    uint64_t bus_prechrg_relay_st : 2;   // BF состояние реле предзаряда шины
    uint64_t center_con_st        : 2;   // BF состояние центрального контактора
    uint64_t bus_actv_isol_st     : 4;   // BF состояние активного теста изоляции
    uint64_t bus_pasv_isol_st     : 4;   // BF состояние пассивного теста изоляции
    uint64_t hvil_st              : 2;   // BF состояние контура блокировки HVIL
    uint64_t acc_st               : 2;   // BF состояние датчика инерции(акселерометра)
    uint64_t soc_st               : 4;   // BF состояние soc
    uint64_t cell_bal_st          : 2;   // BF "сбалансированность" системы
    uint64_t cell_bal_actv        : 2;   // BF состояние процесса балансировки
    uint64_t int_chrgr_st         : 4;   // BF состояние внутреннего зарядного устройства
    uint64_t hves_st_counter      : 4;   // BF контрольный таймер "зависания"
    uint64_t bus_conn_st          : 4;   // BF состояние подключения СНЭ к шине
    uint64_t hves_oper_st         : 4;   // BF режим работы СНЭ
    uint64_t num_of_esp_rdy       : 8;   // 1 число параллельно подключенных МБ к шине
    uint64_t hves_st_crc          : 8;   // 1 контрольная сумма
    } status1;
    // Status2
    struct
    {
    uint64_t dpl_soc      : 2;           // BF ограничение мощности разряда из-за soc
    uint64_t dpl_temp     : 2;           // BF ограничение мощности разряда из-за температуры
    uint64_t dpl_bat_diag : 2;           // BF ограничение мощности разряда из-за диагностики батареи
    uint64_t dpl_volt     : 2;           // BF ограничение мощности разряда из-за напряжения СНЭ или отдельной ячейки
    uint64_t dpl_cur      : 2;           // BF ограничение мощности разряда из-за тока батареи
    uint64_t dpl_undef    : 2;           // BF ограничение мощности разряда из-за неопределенной причины
    uint64_t dpl_res      : 20;          // BF резерв
    uint64_t cpl_soc      : 2;           // BF ограничение мощности заряда из-за soc
    uint64_t cpl_temp     : 2;           // BF ограничение мощности заряда из-за температуры
    uint64_t cpl_bat_diag : 2;           // BF ограничение мощности заряда из-за диагностики батареи
    uint64_t cpl_volt     : 2;           // BF ограничение мощности заряда из-за напряжения СНЭ или отдельной ячейки
    uint64_t cpl_cur      : 2;           // BF ограничение мощности заряда из-за тока батареи
    uint64_t cpl_undef    : 2;           // BF ограничение мощности заряда из-за неопределенной причины
    uint64_t cpl_res      : 20;          // BF резерв
    } status2;
    // pgn = 65502
    uint32_t intfault;                    // BF слово отказов внутренних
    uint32_t extfault;                    // BF слово отказов внешних
    // pgn = 65503
    uint16_t on;                          // 1   число включений
    uint16_t moto;                        // час время наработки
    uint16_t autonom_run;                 // км  пробег в режиме автономного хода
    uint16_t total_run;                   // км  общий пробег
    // pgn = 65504
    uint16_t num_of_charges;              // 1  число зарядок
    uint16_t energy_in;                   // Ah принятая энергия
    uint16_t energy_out;                  // Ah выданная энергия
    uint16_t energy_heat;                 // Ah выданная энергия
    // pgn = 65505
    uint16_t num_of_fault_disc;           // число аварийных отключений
    uint16_t speed;                       // км/ч
    // pgn = 65506
    uint32_t state_in;
    uint32_t state_out;
} __attribute__ ((__packed__)) mb100_ram_struct;

typedef struct {
    uint32_t           start;
    tms_struct         tms;
    bat_struct         bat;
    mb100_ram_struct   mb;
    sma_struct         sma[14];
    cell_struct        cell[14][12];
    uint8_t            res[24];
    uint32_t           crc32;
}  __attribute__ ((__packed__)) sdcard_packet100_struct;

typedef union {
    sdcard_packet100_struct packet100;


} packet_union;

f2d_packet_version_enum f2d_openfile(const char *filename);
f2d_status f2d_get_next_packet(rec_struct *rec);
f2d_status f2d_read_packet(packet_union *packet, size_t len);
void f2d_data100(packet_union *packet, rec_struct *rec);

#endif