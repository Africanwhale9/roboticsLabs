#pragma config(StandardModel, "EV3_REMBOT")
float tolerance;
int increment;
int desiredPowerL;
int desiredPowerR;
int currentPowerR;
int currentPowerL;

float desiredSpeedL;
float desiredSpeedR;
float currentSpeedL;
float currentSpeedR;

int powerIncrement;



int convertRad(float x, tMotor motor){

	return sgn(getMotorSpeed(motor))*((x/60)*360);
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
	}

void angularVelControl(float *desiredSpeed, float *currentSpeed tMotor motor, int *desiredPower, int *currentPower){//desired power may need to be a pointer

	while(true){
		*currentSpeed = convertRad(getMotorRPM(motor), motor);


		if (abs(*desiredSpeed - *currentSpeed) > tolerance){
			// need to increase and decrease via slew rate. How do i know what power to use?
			if (*currentSpeed < *desiredSpeed){
				*desiredPower+=powerIncrement;
				motorspeed(*desiredPower, currentPower);//increase desired power
			}
			else{//decrease speed
				*desiredPower-=powerIncrement;
				motorspeed(*desiredPower, currentPower);
			}
		}
		sleep(250);
	}
}



task motorControlTaskL()
{
	// while (true){
	// 	motorspeed(desiredPowerL,&currentPowerL);
	// }
	angularVelControl(&desiredSpeedL, &currentSpeedL, leftMotor, &desiredPowerL, &currentPowerL);

}

task motorControlTaskR()
{
	// while (true){
	// 	motorspeed(desiredPowerR,&currentPowerR);
	// }
	angularVelControl(&desiredSpeedR, &currentSpeedR, rightMotor, &desiredPowerR, &currentPowerR);


}



void go(){
	setMotorSpeed(leftMotor, currentPowerL);
	setMotorSpeed(rightMotor, currentPowerR);
}


task gotask()
{
	while (true){
		go();
	}
}

task display()
{
	while (true){

		displayBigTextLine(0,"RMotor currentSpeed/desiredSpeed: %f/%f",currentSpeedR, desiredSpeedR);//needs to print floats, not ints (%d)
		displayBigTextLine(8,"LMotor currentSpeed/desiredSpeed: %f/%f",currentSpeedL, desiredSpeedL);
	}
}


task main(){
	tolerance = .05;
	powerIncrement = 10;

	desiredSpeedL = 360;
	desiredSpeedR = 360;

	startTask(display, 10);
	startTask(gotask, 10);
	startTask(motorControlTaskR, 10);
	startTask(motorControlTaskL, 10);

}
