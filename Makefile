
all: decode_cxe
	./decode_cxe ./files/18_53.CXE

decode_cxe: decode_cxe.o crc32.o
	gcc decode_cxe.o crc32.o -o decode_cxe 

decode_cxe.o: decode_cxe.c
	gcc -c decode_cxe.c 

crc32.o: crc32.c crc32.h
	gcc -c crc32.c
	

clean:
	rm -rf *.o decode_cxe