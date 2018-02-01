#pragma config(StandardModel, "EV3_REMBOT");

int currentAngle;
float desiredAngle;
int desiredRadian;
float leftAngularVel;
float rightAngularVel;
float Xr;
float A;
float diameter;
float l;


float getAngularVelRad(tMotor motor){


	return ((getMotorRPM(motor)*(2*PI))/60);
}

void anglechange(){
	if(desiredAngle<0){
		setMotorSyncEncoder(leftMotor, rightMotor, 100, desiredAngle, 20);

	}
	else{
		setMotorSyncEncoder(leftMotor, rightMotor, -100, desiredAngle, 20);
	}
	sleep(250);
}

task display(){
	while (true){
		leftAngularVel = getAngularVelRad(leftMotor);
		rightAngularVel =getAngularVelRad(rightMotor);


		Xr = (diameter/2)*(rightAngularVel+leftAngularVel);
		A = (diameter/(2*l))*(rightAngularVel-leftAngularVel);

		displayTextLine(5,"Desired Angle: %d",desiredAngle);
		displayTextLine(7,"Current Angle: %d",currentAngle);
		displayTextLine(9,"the error %d",desiredAngle-currentAngle);
		displayTextLine(10, "A: %f", A);
		displayTextLine(12, "Xr: %f", Xr);

	}
}
task go(){
	anglechange();
}

task updateangle(){
	while(true){
		currentAngle= getMotorEncoder(rightMotor);
	}
}



task main(){

	diameter = 58;
	l = 60;

	currentAngle=0;
	desiredRadian=-15;
	desiredAngle= desiredRadian*56.7;
	startTask(display, 10);
	startTask(go, 10);
	startTask(updateangle, 10);
	while(true){}
}
