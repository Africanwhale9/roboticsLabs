#pragma config(StandardModel, "EV3_REMBOT");

int dark = 4;
int light = 33;
int current_color;
int turnRatio=40;
int turnPower = 20;
int threshold;
int k = 5;


task display(){
	while(true){
		displayBigTextLine(4, "%d", current_color);
	}
}


task updateColor(){
	while(true){
		current_color = getColorReflected(colorSensor);
	}
}

task followLine(){
	while(true){
		if(current_color>threshold+k){//in light
			if(turnRatio<0){
				turnRatio=turnRatio*-1;
			}
		}

		if(current_color<threshold-k){//in dark
			if(turnRatio>0){
				turnRatio=turnRatio*-1;
			}
		}



		setMotorSync(leftMotor, rightMotor, turnRatio, turnPower);
	}
}



task main()
{
	threshold = (dark+light)/2;
	startTask(display, 7);
	startTask(updateColor, 7);
	startTask(followLine, 7);
	while(true){}


}
