#include <stdio.h>

#include "crc32.h"
#include "file2data.h"

static FILE *ifile = NULL;
static f2d_packet_version_enum version = F2D_PACKET_ERROR;
//-----------------------------------------------------------------------------
f2d_packet_version_enum f2d_openfile(const char *filename) {
    version = F2D_PACKET_ERROR;
    if(ifile != NULL)  fclose(ifile);
    ifile = fopen(filename, "rb");
    if(ifile == NULL) {        
        printf("file not found\n");
        version = F2D_PACKET_FILE_ERROR;
        fclose(ifile);
    }
    else {
        packet_union packet;
        fread(&packet, sizeof(packet), 1, ifile);
        if(CRC32_Soft((uint32_t*)&packet, (40>>2))   == 0) { version = F2D_PACKET_VERSION_1000;}
        if(CRC32_Soft((uint32_t*)&packet, (1300>>2)) == 0) { version = F2D_PACKET_VERSION_100; }
        if(CRC32_Soft((uint32_t*)&packet, (1308>>2)) == 0) { version = F2D_PACKET_VERSION_101; }
        if(CRC32_Soft((uint32_t*)&packet, (2544>>2)) == 0) { version = F2D_PACKET_VERSION_102; }
        fseek(ifile, 0, SEEK_SET);          
    }
    return version;
}
//-----------------------------------------------------------------------------
f2d_status f2d_get_next_packet(rec_struct *rec) {
    f2d_status res = F2D_REC_END;
    packet_union packet = {0};
    switch(version) {
        case F2D_PACKET_VERSION_100:  
            //
            res = f2d_read_packet(&packet, 1300);
            if(res == F2D_REC_OK) 
                f2d_data100(&packet, rec);
            break;
        case F2D_PACKET_VERSION_101:  break;
        case F2D_PACKET_VERSION_102:  break;
        case F2D_PACKET_VERSION_1000: break;
        default : break;
    }
    return res;
}
//-----------------------------------------------------------------------------
f2d_status f2d_read_packet(packet_union *packet, size_t len) {
    f2d_status res = F2D_REC_END;
    fread((void*)packet, len, 1, ifile);
    if(CRC32_Soft((uint32_t*)packet, (len>>2)) == 0) 
        res = F2D_REC_OK;
    return res;
}
//-----------------------------------------------------------------------------
void f2d_data100(packet_union *packet, rec_struct *rec) {
    memcpy((void*)&rec->tms[0],        (void*)&packet->packet100.tms,        sizeof(packet->packet100.tms));
    memcpy((void*)&rec->bat,           (void*)&packet->packet100.bat,        sizeof(packet->packet100.bat));
    memcpy((void*)&rec->mb[0],         (void*)&packet->packet100.mb,         sizeof(packet->packet100.mb));
    memcpy((void*)&rec->sma[0][0],     (void*)&packet->packet100.sma[0],     sizeof(sma_struct)*14);
    memcpy((void*)&rec->cell[0][0][0], (void*)&packet->packet100.cell[0][0], sizeof(cell_struct)*14*12);
}