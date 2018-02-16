#pragma config(StandardModel, "EV3_REMBOT");

long degree;
long heading;
long rate;





task updateVariables(){
	while(true){
		degree= getGyroDegrees(gyroSensor);
		heading= getGyroHeading(gyroSensor);
		rate= getGyroRate(gyroSensor);

	}
}


task display(){
	while (true){
			displayBigTextLine(2, "Degree: %d", degree);
			displayBigTextLine(4, "Heading: %d ", heading);
			displayBigTextLine(6, "Rate: %d ", rate);
	}
}

task main(){
	resetGyro(gyroSensor);
	startTask(display,7);
	startTask(updateVariables, 7);

	while(true){}


}
