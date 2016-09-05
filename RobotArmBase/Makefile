.PHONY=all

PATHS="/usr/avr/include"

all: RobotArmBaseLib.o RobotArmUart.o

RobotArmBaseLib.o: RobotArmBaseLib.c
	avr-gcc -I${PATHS} -c RobotArmBaseLib.c -o RobotArmBaseLib.o

RobotArmUart.o: RobotArmUart.c
	avr-gcc -I${PATHS} -c RobotArmUart.c -o RobotArmUart.o
