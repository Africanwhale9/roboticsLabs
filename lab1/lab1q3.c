#pragma config(StandardModel, "EV3_REMBOT")

void turn(){
	setMotorSync(leftMotor, rightMotor, 99, 35);
	sleep(1000);
}

void go(){
	setMotorSync(leftMotor, rightMotor, 0, 50);
	sleep(2250);
}

task main()
{

	go();
	turn();
	go();


}
