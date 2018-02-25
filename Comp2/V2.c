#pragma config(StandardModel, "EV3_REMBOT");
float minDist;
long minDegree;
int motorPower = 40;
int EncodersGone;
int sleepTime = 100;
float k = .5;
int slewRate = 1;



void stopMove(){
	setMotorSync(leftMotor,rightMotor, 0, 0);
}


task findMinimumDistance(){
	minDist = getUSDistance(sonarSensor);//must be global variables
	minDegree = getGyroDegrees(gyroSensor);//assume that the start is the minimum we've seen so far
	//USING THIS MEANS WE CANNOT RESET GYRO IN TURN

	while(true){
		int currentDist = getUSDistance(sonarSensor);
		int currentDegree=getGyroDegrees(gyroSensor);
		if(currentDist<minDist){
			minDist=currentDist;
			minDegree = currentDegree;
		}
	}

}



void turn(long degreeOnSensor, int turnPower){//turns so that the sensor reads the passed in degrees, doesn't reset gyro
	int turnRatio = 85;
	int currentPower = 0;
	int minPower = 4;


	long degreeError = degreeOnSensor - getGyroDegrees(gyroSensor);

	if(degreeError<0){//turn right
		turnRatio = turnRatio*-1;


		while(getGyroDegrees(gyroSensor)>degreeOnSensor){
			float degreeError = abs(degreeOnSensor- getGyroDegrees(gyroSensor))*k;
			if(degreeError<turnPower){//hoping it won't enter this until it accelerates a bit
				if(degreeError<minPower){
					degreeError = minPower;
				}
				setMotorSync(leftMotor, rightMotor, turnRatio, degreeError);
			}

			else{
				currentPower+=slewRate;
				if(currentPower>turnPower){
					currentPower=turnPower;
				}
				setMotorSync(leftMotor, rightMotor, turnRatio, currentPower);
				sleep(10);
				//do nothing, keep turning

			}
			displayTextLine(1, "%ld", getGyroDegrees(gyroSensor));
		}
	}




	else{//turn left



		setMotorSync(leftMotor,rightMotor, turnRatio, turnPower);

		while(getGyroDegrees(gyroSensor)<degreeOnSensor){
			float degreeError = abs(degreeOnSensor- getGyroDegrees(gyroSensor))*k;
			if(degreeError<turnPower){//hoping it won't enter this until it accelerates a bit
				if(degreeError<minPower){
					degreeError = minPower;
				}
				setMotorSync(leftMotor, rightMotor, turnRatio, degreeError);
			}

			else{
				currentPower+=slewRate;
				if(currentPower>turnPower){
					currentPower=turnPower;
				}
				setMotorSync(leftMotor, rightMotor, turnRatio, currentPower);
				sleep(10);
				//do nothing, keep turning

			}
			displayTextLine(1, "%ld", getGyroDegrees(gyroSensor));
			//do nothing, keep turning
		}
	}



	stopMove();//we have turned the required amount, so stop

}



void scan(){
	resetGyro(gyroSensor);

	startTask(findMinimumDistance, 7);

	turn(45, motorPower); //turn left 45 degrees

	sleep(250);

	turn(-45, motorPower); //turn right so its 45 degrees from the origin

	sleep(100);

	stopTask(findMinimumDistance);

	turn(minDegree, motorPower);//this should have the robot rotate to the closest distance (ie facing the wall square on);

	resetGyro(gyroSensor);

}









task AccumulateEncoderValues(){//to keep track of how far the robot goes in encoder values
	EncodersGone=0;
	resetMotorEncoder(leftMotor);
	while(true){
		EncodersGone += getMotorEncoder(leftMotor);
		resetMotorEncoder(leftMotor);
		sleep(5);
	}

}


void moveToDistance(int distFromWall, int power){//the use of this function is to get the robot to a certain distance away from the wall
	int currentPower = 0;
	float error;
	int minPower = 4;
	float gain = 1;

	while(getUSDistance(sonarSensor)>distFromWall){
		error = abs(getUSDistance(sonarSensor) - distFromWall)*gain;
		if(error<power){
			if(error<minPower){
				error=minPower;
			}
			setMotorSync(leftMotor,rightMotor, 0, error);
		}
		else{
			currentPower+=slewRate;
			if(currentPower>power){
				currentPower=power;
			}
			setMotorSync(leftMotor,rightMotor, 0, currentPower);

		}
	}
	stopMove();//robot has reached required distance, stop

}


task main(){

	resetGyro(gyroSensor);

	//turn(-45, motorPower);
	//sleep(100);
	//turn(45, motorPower);

	scan();//this squares it to the wall

	sleep(sleepTime);

	moveToDistance(20, motorPower);//moves within 15 mm of the wall

	resetGyro(gyroSensor);
	turn(180, motorPower);//turns 180
	resetGyro(gyroSensor);

	sleep(sleepTime);


	startTask(AccumulateEncoderValues, 7);

	moveToDistance(15, motorPower); //moves to within 15mm of the opposite wall

	stopTask(AccumulateEncoderValues);


	sleep(sleepTime);

	resetGyro(gyroSensor);
	turn(180, motorPower); //turns 180
	resetGyro(gyroSensor);

	sleep(sleepTime);

	setMotorSyncEncoder(leftMotor,rightMotor, 0, EncodersGone/2, motorPower); //moves to center of the walls

	while(true){}
}
