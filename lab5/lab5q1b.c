#pragma config(StandardModel, "EV3_REMBOT");
int threshold = 29;
int speed=25;

task moves(){
	while(true){
		if(getColorReflected (colorSensor) > threshold){
			setMotorSpeed(leftMotor, speed);
			setMotorSpeed(rightMotor, speed);
			}else{
				speed=speed*(-1);
				setMotorSpeed(leftMotor, speed);
				setMotorSpeed(rightMotor, speed);
				sleep(1000);
		}
	}
}

task display(){
	while (true){
		displayBigTextLine(2, "%d", getColorReflected(colorSensor));
	}
}

task main(){
	startTask(moves, 7);
	startTask(display, 7);

	while(true){}


}
