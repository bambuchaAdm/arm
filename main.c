#include "./RobotArmBase/RobotArmBaseLib.h"

// (x,y)

const uint8_t bufferSize = 3;


int main(){
	initRobotBase();
	Servo_Power_And_Start();

	mSleep(1000);

	char receiveBuffer[bufferSize];


	receiveBytes(bufferSize);
	waitUntilReceptionComplete();
	copyReceivedBytesToBuffer(&receiveBuffer[0]);

	while(1){

	}
}
