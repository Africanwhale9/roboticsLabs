#pragma config(StandardModel, "EV3_REMBOT")

void turnRight(){
	setMotorSync(leftMotor, rightMotor, 99, 38);
	sleep(500);
}

void turnLeft(){
	setMotorSync(leftMotor, rightMotor, -99, 38);
	sleep(500);
}

void go(int x){
	setMotorSync(leftMotor, rightMotor, 0, 50);
	sleep(x);
}

task main()
{
	go(1912);
	turnLeft();
	go(2040);
	turnLeft();
	go(1173);
	turnLeft();
	go(306);
	turnRight();
	go(2040);
	turnLeft();
	go(1912);


}
