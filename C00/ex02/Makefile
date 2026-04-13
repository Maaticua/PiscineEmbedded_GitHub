MCU = atmega328p
F_CPU = 16000000
BIN = main.bin
HEX = main.hex
SRC = main.c


all: hex flash

hex:
		avr-gcc -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os  -c $(SRC) -o main.o
		avr-gcc -mmcu=$(MCU) -e main main.o -o $(BIN)
		avr-objcopy -O ihex $(BIN) $(HEX)

flash:
		avrdude -c arduino -p $(MCU) -P /dev/ttyUSB0 -b 115200 -U flash:w:$(HEX)

clean:
		rm -f *.o $(BIN) $(HEX)