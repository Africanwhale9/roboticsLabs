#pragma config(StandardModel, "EV3_REMBOT");
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



float convertRad(float x, tMotor motor){


	return ((x*(2*PI))/60)*sgn(getMotorSpeed(motor));
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

void angularVelControl(float *desiredSpeed, float *currentSpeed, tMotor motor, int *desiredPower, int *currentPower){

	while(true){
		*currentSpeed = convertRad(getMotorRPM(motor), motor);


		if (abs(*desiredSpeed - *currentSpeed) > tolerance){
			// need to increase and decrease via slew rate. How do i know what power to use?
		//	displayTextLine(15,"%f",abs(*desiredSpeed - *currentSpeed));
			if (*currentSpeed < *desiredSpeed){
				*desiredPower+=powerIncrement;
				motorspeed(*desiredPower, currentPower);//increase desired power
			}
			else{//decrease speed
				*desiredPower-=powerIncrement;
				motorspeed(*desiredPower, currentPower);
			}
		}
		sleep(50);
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

		displayTextLine(0,"RMotor: %f/%f",currentSpeedR, desiredSpeedR);//needs to print floats, not ints (%d)
		displayTextLine(8,"LMotor: %f/%f",currentSpeedL, desiredSpeedL);
	}
}


task main(){
	tolerance = .05;
	powerIncrement = 1;
	increment = 1;

	desiredSpeedL = 10;
	desiredSpeedR = 10;

	desiredPowerL=5;
	desiredPowerR=5;

	startTask(display, 10);
	startTask(gotask, 10);
	startTask(motorControlTaskR, 10);
	startTask(motorControlTaskL, 10);
	while(true){
	}

}
