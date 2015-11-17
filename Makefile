CC=avr-gcc
SRC=serial

$(SRC): $(SRC).c
	$(CC) -o $(SRC).out -Os $(SRC).c -mmcu=atmega328p
	avr-objcopy -O binary $(SRC).out $(SRC).bin
	avrdude -p m328p -P /dev/ttyACM0 -c arduino -U flash:w:$(SRC).bin

