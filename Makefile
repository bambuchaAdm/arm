

main.elf: main.o 
	make -C RobotArmBase
	avr-gcc -I/usr/avr/include main.o RobotArmBase/RobotArmBaseLib.o RobotArmBase/RobotArmUart.o

main.o: main.c
	avr-gcc -c main.c -o main.o

