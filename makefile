
OBJS=uart.cpp.o main.o uart-crsf.o read_data.o
CC=gcc
CFLAGS=-c -Wall -g
 
crsf-app:$(OBJS)
	$(CC) $^ -o crsf-app

main.o:main.cpp
	$(CC) $^ $(CFLAGS)  -o $@

read_data.o:read_data.cpp
	$(CC) $^ $(CFLAGS)  -o $@
	
uart.cpp.o:uart.cpp
	$(CC) $^ $(CFLAGS)  -o $@

uart-crsf.o:uart-crsf.cpp
	$(CC) $^ $(CFLAGS)  -o $@
clean:
	$(RM) *.o crsf-app -r
