
main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex

main.elf: main.o 
	avr-gcc -mmcu=atmega16 -I. -gdwarf-2 -DF_CPU=16000000UL -Os -funsigned-char main.o RobotArmBase/RobotArmBaseLib.o RobotArmBase/RobotArmUart.o -o main.elf

main.o: main.c
	avr-gcc -c -mmcu=atmega64 -I. -I/usr/avr/include -gdwarf-2 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fshort-enums -Wall \
-Wstrict-prototypes  -std=gnu99 main.c

