#pragma config(StandardModel, "EV3_REMBOT")
int desiredPowerL;
int desiredPowerR;
int currentPowerR;
int currentPowerL;
int increment;

void rannum(){
	 desiredPowerL=abs((rand())%100+1);
	 desiredPowerR=abs((rand())%100+1);
}
void motorspeed(int desiredPower, int *currentPower){
		while (*currentPower != desiredPower){
			if (desiredPower!=*currentPower){
				if (*currentPower < desiredPower){
					if(*currentPower + increment > desiredPower){
						*currentPower = desiredPower;
					}
					else{
						*currentPower += increment;
					}
				}
				else{
					if(*currentPower - increment > desiredPower){
						*currentPower = desiredPower;
					}
					else{
						*currentPower -= increment;
					}
				}
			}
			sleep(100);
		}
		rannum();
	}

void go(){
	setMotorSpeed(leftMotor, currentPowerL);
	setMotorSpeed(rightMotor, currentPowerR);
}

task display()
{
	while (true){
		displayBigTextLine(8,"LMotor %d/%d",currentPowerL, desiredPowerL);
		displayBigTextLine(4,"RMotor%d/%d",currentPowerR, desiredPowerR);
	}
}

task motorControlTaskL()
{
	while (true){
		motorspeed(desiredPowerL,&currentPowerL);
	}
}

task motorControlTaskR()
{
	while (true){
		motorspeed(desiredPowerR,&currentPowerR);
	}
}

task gotask()
{
	while (true){
	go();
	}
}



task main()
{
	currentPowerL=0;
	desiredPowerL=100;

	currentPowerR=0;
	desiredPowerR=100;

	increment=10;
	startTask(display, 10);
	startTask(motorControlTaskL, 10);
	startTask(motorControlTaskR, 10);
	startTask(gotask,10);
	while(true){}
}
