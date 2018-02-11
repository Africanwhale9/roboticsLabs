#pragma config(StandardModel, "EV3_REMBOT");
float distance;



task range(){
	while(true){
		distance = getUSDistance(sonarSensor);

	}
}

task display(){
	while(true){
		displayBigTextLine(8, "%f cm", distance);
	}

}

task main()
{
	startTask(range, 7);
	startTask(display, 7);
	while(true){}



}
