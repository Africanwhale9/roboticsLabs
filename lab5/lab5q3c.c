#pragma config(StandardModel, "EV3_REMBOT");
float desiredDeg;
float currentDeg;
float error;
int desiredPower=25;
int currentPower;
int slewRate = 1;
float k =1;
float tolerance = 1;
int dir;

task slew(){
	while (true){
		error = desiredDeg - currentDeg;
		if(abs(error)>tolerance){//this should take care of the direction automatically
			desiredPower = k*error*-1;//this will be negative if currentDist>disireddist (must flip the sign)
			}else{
			desiredPower=0;
		}
		if(error<0){//error calculation already tells us which way the robot needs to move, if error is negative, it must move forward
			if((currentPower + slewRate) < desiredPower){
				currentPower += slewRate;
				}else{
				currentPower = desiredPower;
				}if (currentPower> desiredPower){
				currentPower = desiredPower;
			}
		}
	}
}

task motorControl(){
	while(true){
		setMotorSync(leftMotor, rightMotor, -100, desiredPower);

	}
}

task update(){
	while (true){
		currentDeg=getGyroDegrees(gyroSensor);
	}
}
task display(){
	while (true){

		displayBigTextLine(4, "DesDegree: %d", desiredDeg);
		displayBigTextLine(6, "CurrPower: %d", currentPower);
		displayBigTextLine(8, "DesPower: %d", desiredPower);
		displayBigTextLine(10, "Error: %d", error);
	}
}


void turn(int deg){
	currentDeg= getGyroDegrees(gyroSensor);
	desiredDeg=deg+currentDeg;
	startTask(slew,7);
	startTask(motorControl,7);
	startTask(update,7);
}

task main(){
	startTask(display,7);
	turn(-60);
	while (true){}
}
