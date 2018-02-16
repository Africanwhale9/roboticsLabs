#pragma config(StandardModel, "EV3_REMBOT");
int ratio = 100;
bool isMove = false;


task turn(){
	setMotorSyncEncoder(leftMotor, rightMotor, ratio,825, 100);
	ratio = ratio*-1;
}


task isMoving(){
	while(true){
		if(getMotorRPM(leftMotor)!=0 || getMotorRPM(rightMotor)!=0){
			isMove = true;
		}
		else{
			isMove = false;
		}
	}
}


task didBump(){
	while (true){
		if(isMove){
			if(getBumpedValue(touchSensor)){
				setMotorSync(leftMotor, rightMotor, 0, 0);
				resetBumpedValue(touchSensor);
				stopTask(turn);
			}
		}
	}
}


task main(){
	startTask(isMoving, 7);
	startTask(didBump, 7);
	while(true){
		if(getBumpedValue(touchSensor) && !isMove){
			resetBumpedValue(touchSensor);
			startTask(turn, 7);


		}
	}
}
