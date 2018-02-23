#pragma config(StandardModel, "EV3_REMBOT");

float mmperdeg;
float distToDeg;
float diameter=58;
float l=60;
float A;
float theta;

void foward(float desiredDist){
	distToDeg= desiredDist/mmperdeg;
	setMotorSyncEncoder(leftMotor, rightMotor, 0, (distToDeg), 50);
	sleep(5000);
}


void turn(float desiredAngle){
	theta+=desiredAngle;
	if(desiredAngle<0){
		setMotorSyncEncoder(leftMotor, rightMotor, 100, ((desiredAngle*2)), 50);
	}
	else{
		setMotorSyncEncoder(leftMotor, rightMotor, -100, ((desiredAngle*2)), 50);
	}
	sleep(5000);
}

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

task display(){
	float rightAngularVel;
	float leftAngularVel;
	while (true){
		leftAngularVel = getAngularVelRad(leftMotor);
		rightAngularVel =getAngularVelRad(rightMotor);

		A = (diameter/(2*l))*(rightAngularVel-leftAngularVel);

		displayTextLine(8, "Dot Theta: %f", A);
		displayTextLine(12, "Theta: %f", theta);

	}
}


task main(){
startTask(display,10);
mmperdeg=182.21/365;

turn(60);
foward(500);
turn(-90);
foward(1000);
turn(150);
foward(300);





while(true){}
}
