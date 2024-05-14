# CC: компилятор для программ C; по умолчанию это cc.
# CXX: компилятор для программ C++; по умолчанию это g++.
# CFLAGS: дополнительные флаги для компилятора C.
# CXXFLAGS: дополнительные флаги для компилятора C++.
# CPPFLAGS: дополнительные флаги для препроцессора C.
# LDFLAGS: дополнительные флаги для компиляторов, когда подразумевается, что они вовлекают в процесс компиляции запуск линкера.
CC = gcc    # компилятор
CFLAGS = -g -Wall # флаг для неявных правил. Он включит отладочную информацию
                  # в результат компиляции


all: decode_cxe	
	rm -rf ./files/*.csv
	./decode_cxe ./files/11_41.CXE
	./decode_cxe ./files/AA_11_37.CXE	
	
decode_cxe: decode_cxe.o crc32.o data2file.o file2data.o param.o
	$(CC) $(CFLAGS) crc32.o decode_cxe.o data2file.o file2data.o param.o  -o decode_cxe 

decode_cxe.o: crc32.h decode_cxe.c file2data.h param.h
	$(CC) $(CFLAGS) -c decode_cxe.c 

file2data.o: file2data.c file2data.h crc32.h
	$(CC) $(CFLAGS) -c file2data.c	

crc32.o: crc32.c crc32.h
	$(CC) $(CFLAGS) -c crc32.c

param.o: param.c param.h
	$(CC) $(CFLAGS) -c param.c

data2file.o: data2file.c data2file.h
	$(CC) $(CFLAGS) -c data2file.c	

clean:
	rm -rf *.o decode_cxe
	rm -rf ./files/*.csv