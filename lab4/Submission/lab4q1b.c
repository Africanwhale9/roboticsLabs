#pragma config(StandardModel, "EV3_REMBOT");
int ratio = 100;


void turn(){
	setMotorSyncEncoder(leftMotor, rightMotor, ratio,825, 100);
	ratio = ratio*-1;
}

task main(){
	while(true){
		if(getBumpedValue(touchSensor)){
			turn();
			resetBumpedValue(touchSensor);
		}
	}
}
