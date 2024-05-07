
all: decode_cxe
	./decode_cxe ./files/18_53.CXE

decode_cxe: decode_cxe.o crc32.o file2data.o param.o
	gcc crc32.o decode_cxe.o file2data.o param.o  -o decode_cxe 

decode_cxe.o: crc32.h decode_cxe.c file2data.h param.h
	gcc -c decode_cxe.c 

file2data.o: file2data.c file2data.h crc32.h
	gcc -c file2data.c	

crc32.o: crc32.c crc32.h
	gcc -c crc32.c

param.o: param.c param.h
	gcc -c param.c
	

clean:
	rm -rf *.o decode_cxe