#pragma config(StandardModel, "EV3_REMBOT");

float mmperdeg;
float distToDeg;

void foward(float desiredDist){
	distToDeg= desiredDist/mmperdeg;
	setMotorSyncEncoder(leftMotor, rightMotor, 0, (distToDeg), 50);
	sleep(5000);
}


void turn(float desiredAngle){
	if(desiredAngle<0){
		setMotorSyncEncoder(leftMotor, rightMotor, 100, ((desiredAngle*2)), 50);

	}
	else{
		setMotorSyncEncoder(leftMotor, rightMotor, -100, ((desiredAngle*2)), 50);
	}
	sleep(5000);
}

task main()
{
mmperdeg=182.21/365;

//turn(60);
//foward(500);
//turn(-90);
//foward(1000);
//turn(150);
//foward(300);





while(true){}
}
