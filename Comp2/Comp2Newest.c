#pragma config(StandardModel, "EV3_REMBOT");
float max_dist=20; //mm
float short_dist=30;
float cur_dist;
float dist1;
float dist2;
float total_dist;
const int bufferSize = 3;
float distBuffer[bufferSize];
int ratio= -100;
bool did_enter=false;

void stop(){
	setMotorSync (leftMotor, rightMotor, 0, 0);
}

void find_wall(){
	while (cur_dist>=max_dist)	{
		setMotorSync(leftMotor, rightMotor, 0, 20);
	}
	stop();
}

task turn(){
	while (true){
		setMotorSync(leftMotor, rightMotor, ratio, 5);
	}
}

void square(){
	startTask (turn,7);
	while(cur_dist<short_dist){
		short_dist= cur_dist;
		did_enter=true;
		sleep(100);
	}
	stopTask(turn);
	stop();
	if (! did_enter){
		ratio=ratio*-1;
		square();
	}
}


task display(){
	while (true){
		displayBigTextLine(10,"%d", cur_dist);
	}
}

void insertDistBuffer(float var){
	int i;
	for(i=0;i<bufferSize-1;i++){
		distBuffer[i]=distBuffer[i+1];
	}
	distBuffer[bufferSize-1]=var;
}


float average(){
	float x = 0;
	int i;
	for(i=0;i<bufferSize-1;i++){
		x+=distBuffer[i];
	}
	return x/bufferSize;
}

task update_dist(){
	while (true){
		insertDistBuffer(getUSDistance(sonarSensor));
		sleep(100);
		cur_dist=average();

	}
}


task main(){
	startTask(display,7);
	startTask (update_dist,7);
	sleep(1000);
	find_wall();
	displayBigTextLine(12,"found wall");
	square();
	displayBigTextLine(14,"square");

	while (true){}
}
