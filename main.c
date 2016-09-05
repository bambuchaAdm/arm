#include "./RobotArmBase/RobotArmBaseLib.h"

// (x,y)

const uint8_t bufferSize = 3;

int main(void){
	initRobotBase();

	while(1){
		writeString_P("Ala ma kota\n");
		mSleep(200);
	}
}
