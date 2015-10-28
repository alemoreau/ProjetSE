CC=avr-gcc

spi: spi.c
	$(CC) -o spi.out -Os spi.c -mmcu=atmega328p
	avr-objcopy -O binary spi.out spi.bin
	avrdude -p m328p -P /dev/ttyACM0 -c arduino -U flash:w:spi.bin
