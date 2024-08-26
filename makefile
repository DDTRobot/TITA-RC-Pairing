OBJS=main.o uart_target.o uart-crsf.o read_data.o  
CC=gcc  
CFLAGS=-c -Wall -g  
  
# 根据环境变量 UART_SOURCE 决定是否编译 uart.cpp 或 uart_ubuntu.cpp  
ifeq ($(UART_SOURCE),0)  
UART_OBJ=uart.cpp.o  
UART_SRC=uart.cpp  
else  
UART_OBJ=uart_ubuntu.cpp.o  
UART_SRC=uart_ubuntu.cpp  
endif  
  
crsf-app: $(OBJS)  
	$(CC) $^ -o crsf-app  
  
main.o: main.cpp  
	$(CC) $(CFLAGS) $^ -o $@  
  
read_data.o: read_data.cpp  
	$(CC) $(CFLAGS) $^ -o $@  
  
uart-crsf.o: uart-crsf.cpp  
	$(CC) $(CFLAGS) $^ -o $@  
  
# 根据 UART_SOURCE 变量编译 uart.cpp 或 uart_ubuntu.cpp  
$(UART_OBJ): $(UART_SRC)  
	$(CC) $(CFLAGS) $^ -o $@  
  
clean:  
	rm -f *.o crsf-app  
  
.PHONY: clean