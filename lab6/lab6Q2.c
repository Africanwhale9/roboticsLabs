#pragma config(StandardModel, "EV3_REMBOT");

int threshold;
int dark = 7;
int light = 70;
int current_color;
float error;
float desiredChange;


int motorPower = 20;
float k = 1.99;

//int motorPower = 40;
//float k = 1.5;



task display(){
	while(true){
		displayBigTextLine(4, "%d", current_color);
		displayBigTextLine(6, "threshold %d", threshold);
		displayBigTextLine(8, "desCh%d", desiredChange);
	}
}


task updateColor(){
	while(true){
		current_color = getColorReflected(colorSensor);
	}
}

task motorControl(){
	while(true){
		setMotorSync(leftMotor, rightMotor, desiredChange, motorPower);
	}
}

task updateDesiredChange(){
	while(true){
		error = current_color-threshold;
		desiredChange = k*error;
	}
}


task main()
{
	threshold = (dark+light)/2;
	startTask(updateColor, 7);
	startTask(display, 7);
	startTask(updateDesiredChange, 7);
	startTask(motorControl, 7);

	while(true){}


}
