#pragma config(StandardModel, "EV3_REMBOT");
float tolerance;
int increment;

int lrpm
int rrpm











task display(){
	while (true){
		displayTextLine(0,"RMotor: %f/%f",currentSpeedR, desiredSpeedR);//needs to print floats, not ints (%d)
		displayTextLine(8,"LMotor: %f/%f",currentSpeedL, desiredSpeedL);
	}
}


task main()
{



}
