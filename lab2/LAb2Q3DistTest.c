#pragma config(StandardModel, "EV3_REMBOT");


long encoderGone;
int distanceTravelled;
int circumference;

task main()
{

	circumference = 175.93;
	while (true){
		displayTextLine(8,"%d",distanceTravelled);
		setMotorSync(leftMotor, rightMotor, 0, 10);

		sleep(1000);

		encoderGone = getMotorEncoder(leftMotor);

		distanceTravelled= (encoderGone/360)*circumference;

	}


}
