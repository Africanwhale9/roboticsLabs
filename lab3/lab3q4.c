#pragma config(StandardModel, "EV3_REMBOT");

float mmperdeg;
float desiredDist;
float distToDeg;

void foward(){
	setMotorSyncEncoder(leftMotor, rightMotor, 0, (distToDeg), 50);
}
task go(){
	foward();
}

task main()
{
mmperdeg=182.21/365;
desiredDist=279.4;
distToDeg= desiredDist/mmperdeg;
startTask (go,10);
while(true){}
}
