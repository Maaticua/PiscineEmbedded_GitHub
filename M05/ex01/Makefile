MCU = atmega328p  #defini le model du microcontrolleur
F_CPU = 16000000 #defini la vitesse de lhorloge
BIN = main.bin # nom de fichier binaire
HEX = main.hex # nom du fichier hex
SRC = main.c # le fichier source


all: hex flash # regle lorsqu'on va make, donc hex puis flash

#compile en .o puis en .bin puis en .hex
hex:
		avr-gcc -mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os -c $(SRC) -o main.o
		avr-gcc -mmcu=$(MCU) -e main main.o -o $(BIN)
		avr-objcopy -O ihex $(BIN) $(HEX)

#envoi le code dans la puce, suit le protocole arduino, indique le port usb, la vitesse de communication puis flash le programme dans la carte
flash:
		avrdude -c arduino -p $(MCU) -P /dev/ttyUSB0 -b 115200 -U flash:w:$(HEX)

#supprime tout les fichiers .o .hex .bin
clean:
		rm -f *.o $(BIN) $(HEX)