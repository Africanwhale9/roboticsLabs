#pragma config(StandardModel, "EV3_REMBOT");
int robotLength = 58; //CM
int maxDist = 255;
int distGone;
bool stopturn=false;
bool didEnter = false;
int direction = 100;


void stop(){
	setMotorSync (leftMotor, rightMotor, 0, 0);
}

void slewRateControl(int distanceReq){
	int currentPower = 0;
	int desiredPower = 100;
	int slewRate = 5;
	int error;
	int k = 2;

	while (getUSDistance(sonarSensor)>distanceReq){
		error = getUSDistance(sonarSensor) - distanceReq;
		if(error<desiredPower/2){
			currentPower = error*k;
		}
		else{
			if (currentPower<desiredPower){
				currentPower+=slewRate;
			}
			else{
				currentPower=desiredPower;
			}
		}
		setMotorSync (leftMotor, rightMotor, 0, currentPower);
		sleep(50);
	}

	stop();
}




task slowTurn(){
	setMotorSync(leftMotor, rightMotor, direction, 25);
}




void orient(){
	float prevDist = getUSDistance(sonarSensor);
	startTask(slowTurn, 7);
	sleep(10);
	float currDist = getUSDistance(sonarSensor);

	while (currDist<prevDist){
		didEnter = true;
		prevDist = currDist;
		sleep(10);
		currDist = getUSDistance(sonarSensor);

	}


	stopTask(slowTurn);
	stop();


	if(!didEnter){
		direction= direction*-1;
		orient();
	}

}














void decelerate(){
	int ratio;
	float k = .5;
	float tolerance = .5;

	ratio = -100;

	if(desiredAngle<0){
		ratio = 100;
	}

	float decelpower;
	while (true){
		decel=true;

		if(error<= tolerance){
			error=0;//this should stop the robot once error is within the tolerance range
			setMotorSync (leftMotor, rightMotor, ratio, 0);
			stopturn=true;
			return;
		}



		if( (k* error) < currentPower){
			decelpower=k*error;
		}
		else{
			decelpower=currentPower;
		}
		if( decelpower<7){
			decelpower=7;
		}
		setMotorSync (leftMotor, rightMotor, ratio, decelpower);
		sleep(50);
	}

}



void accelerate(){
	// accelerate until full power or there is only a percentage of the angle left
	int ratio;

	ratio = -100;

	if (desiredAngle<0){
		ratio = 100;
	}

	while((currentPower<desiredPower) ){
		currentPower+=turnRate;
		setMotorSync (leftMotor, rightMotor, ratio, currentPower);
		sleep(100);
	}

}



task goes(){

	accelerate();

	decelerate();

}

task updateangle(){
	while (true){
		currentAngle= getMotorEncoder(rightMotor)/2;
		angleGone = currentAngle/desiredAngle;
		error = abs(desiredAngle-currentAngle);
	}
}


void turn(float deg){
	desiredAngle= deg;
	startTask(goes, 7);
	startTask(updateangle, 7);
	while (true){
		if (stopturn){
			stopTask(goes);
			stopTask(updateangle);
			sleep(500);
			return;
		}
	}
}













task distanceCounter(){
	//use encoders? wheel turns? Sensor?
}


task main(){
	orient();

	slewRateControl(robotLength);

	turn(180);

	if(getUSDistance(sonarSensor)>maxDist){
		startTask(distanceCounter, 7);

		slewRateControl(robotLength);


		turn(180);

		slewRateControl(distGone/2);
	}

	else{
		slewRateControl(getUSDistance(sonarSensor)/2);//if it is within the sensors range then we can just move half the distance detected
	}
}