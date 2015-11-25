CC=avr-gcc

serial: serial.c
	$(CC) -o serial.out -Os serial.c -mmcu=atmega328p
	avr-objcopy -O binary serial.out serial.bin
	avrdude -p m328p -P ${args} -c arduino -U flash:w:serial.bin




