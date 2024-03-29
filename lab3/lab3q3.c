#pragma config(StandardModel, "EV3_REMBOT");

float currentAngle;
float desiredAngle;
float desiredRadian;
float leftAngularVel;
float rightAngularVel;
float Xr;
float A;
float diameter;
float l;
float k;
float error;
float tolerance;
float angleRemaining;//this is a percentage .25 = 25% of angle left
float angleGone;//percentage of angle gone
int currentPower;
int desiredPower;
float turnRate;
bool decel=false;
float decelpower;



float getAngularVelRad(tMotor motor){
	float first;
	float second;
	first = getMotorEncoder(motor);
	sleep(10);
	second = getMotorEncoder(motor);
	float sign =  first-second;

	if (sign<0){
		return -((getMotorRPM(motor)*(2*PI))/60);
	}


	return ((getMotorRPM(motor)*(2*PI))/60);
}

void decelerate(){
	int ratio;

	ratio = -100;

	if(desiredAngle<0){
		ratio = 100;
	}

	//float decelpower;
	while (true){
		decel=true;

		if(error<= tolerance){
			error=0;//this should stop the robot once error is within the tolerance range
			stopAllTasks();
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
		//We probably don't want it to sleep at all, but it may not make a difference
	}

}



void accelerate(){
	// accelerate until full power or there is only a percentage of the angle left
	int ratio;

	ratio = -100;

	if (desiredAngle<0){
		ratio = 100;
	}

	//&&  (angleGone<angleRemaining)
	while((currentPower<desiredPower) ){
		currentPower+=turnRate;
		setMotorSync (leftMotor, rightMotor, ratio, currentPower);
		sleep(100);
	}

}



//void maintain(){
//	while(angleGone<angleRemaining){
//		sleep(20);//may need a setMotorSync here of it at full power at correct ratio
//	}
//}


task display(){
	while (true){
		leftAngularVel = getAngularVelRad(leftMotor);
		rightAngularVel =getAngularVelRad(rightMotor);


		Xr = (diameter/2)*(rightAngularVel+leftAngularVel);
		A = (diameter/(2*l))*(rightAngularVel-leftAngularVel);

		displayTextLine(5,"Desired Angle: %d",desiredAngle);
		displayTextLine(7,"Current Angle: %d",currentAngle);
		displayTextLine(9,"the error %d",error);
		displayTextLine(10, "A: %f", A);
		displayTextLine(12, "Xr: %f", Xr);

	}
}
task go(){

	accelerate();

	// maintain(); this may be unnecessary due to the k*error calculation, if it is above 100 i assume it just puts the motor power at 100

	decelerate();

}

task updateangle(){
	while (true){

		currentAngle= getMotorEncoder(rightMotor)/2;
		angleGone = currentAngle/desiredAngle;
		error = abs(desiredAngle-currentAngle);

		// if(error<= tolerance){
		// 	error=0;
		// 	stopTask (updateangle);//maybe this should be stop all tasks
		// }
	}
}



task main(){

	diameter = 58;
	l = 60;
	k=.5;
	//tolerance=.1*180/PI;
	tolerance=2;
	angleRemaining = 1 - .25;
	turnRate = 3;
	currentPower=0;
	desiredPower=50;



	currentAngle=0;
	//desiredRadian=-((2*PI)/3);
	desiredRadian=2*PI;
	desiredAngle= (abs(desiredRadian*180/PI));
	startTask(display, 10);
	startTask(go, 10);
	startTask(updateangle, 10);
	while(true){}
}
