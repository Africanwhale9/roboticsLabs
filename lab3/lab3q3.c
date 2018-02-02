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


float getAngularVelRad(tMotor motor){


	return ((getMotorRPM(motor)*(2*PI))/60);
}

void anglechange(){
	if(desiredAngle<0){
		setMotorSync (leftMotor, rightMotor, 100, k*error);
	}
	else{
		setMotorSync(leftMotor, rightMotor, -100,k*error);
	}
	sleep(1000);
}

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
	while (true){
		anglechange();
	}
}

task updateangle(){
	while (true){
		currentAngle= getMotorEncoder(rightMotor)/2;
		error = desiredAngle-currentAngle;
		if(error<= tolerance){
			error=0;
			stopTask (updateangle);
		}
	}
}



task main(){

	diameter = 58;
	l = 60;
	k=10;
	tolerance=.1*180/PI;


	currentAngle=0;
	//desiredRadian=-((2*PI)/3);
	desiredRadian=2*PI;
	desiredAngle= desiredRadian*180/PI;
	startTask(display, 10);
	startTask(go, 10);
	startTask(updateangle, 10);
	while(true){}
}
