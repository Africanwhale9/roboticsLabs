#pragma config(StandardModel, "EV3_REMBOT");
float desiredDist=30;
float currentDist;
float error;
int desiredPower=50;
int currentPower;
int slewRate = 5;
int maxPower=50;
float k =10;
float tolerance = 2;




task newController(){
	error = desiredDist - currentDist;
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
		}if (currentPower> maxPower){
			currentPower = maxPower;
		}
	}else{//the robot must move backward
		if((currentPower - slewRate) > desiredPower){
			currentPower -= slewRate;
		}else{
			currentPower = desiredPower;
		}if (currentPower< -maxPower){
			currentPower = -maxPower;
		}
	}
}


task range(){
	while(true){
		currentDist = getUSDistance(sonarSensor);
	}
}

task display(){
	while(true){
		displayBigTextLine(8, "%f cm", currentDist);
	}
}

task motorControl(){
	while(true){
		setMotorSpeed(leftMotor, currentPower);
		setMotorSpeed(rightMotor, currentPower);
	}
}

task main(){
	startTask(display, 7);
	startTask(range, 7);
	startTask(motorControl, 7);

	while(true){
		if(currentDist<60){
			startTask(newController, 7);
			sleep(100);
		}
		else{
			currentPower=0;
		}
	}
}



// task controllerTask(){
// 	error = desiredDist - currentDist;

// 	if(abs(error)>tolerance){
// 		desiredPower = k*error;
// 	}
// 	else{
// 		desiredPower = 0;
// 	}

// 	if (currentDist>desiredDist){//it should move forward
// 		if((currentPower + slewRate) < desiredPower){
// 			currentPower += slewRate;
// 		}
// 		else{
// 			currentPower = desiredPower;
// 		}

// 		if (currentPower> maxPower){
// 			currentPower = maxPower;
// 		}

// 		else{//move backward
// 			if((currentPower - slewRate) > desiredPower){
// 				currentPower -= slewRate;
// 			}
// 			else{
// 				currentPower = desiredPower;
// 			}

// 			if (currentPower< -maxPower){
// 				currentPower = -maxPower;
// 			}
// 		}
// 	}
// }
