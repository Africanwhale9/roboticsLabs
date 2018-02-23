#pragma config(StandardModel, "EV3_REMBOT");
float max_dist=20; //mm
float short_dist=30;
float cur_dist;
float dist1;
float dist2;
float total_dist;
const int bufferSize = 3;
float distBuffer[bufferSize];

void stop(){
	setMotorSync (leftMotor, rightMotor, 0, 0);
}

void turn_left(){
	while(cur_dist>=max_dist){
		setMotorSync(leftMotor, rightMotor, -100, 20);
	}
	stop();
}

void turn_right(){
	while(cur_dist>=max_dist){
		setMotorSync(leftMotor, rightMotor, 100, 20);
	}
	stop();
}

void find_wall(){
	while (cur_dist>=max_dist)	{
		setMotorSync(leftMotor, rightMotor, 0, 20);
	}
	stop();
}
void square(){
	resetGyro(gyroSensor);
	while (cur_dist<=short_dist){
		if(cur_dist<short_dist){
			short_dist=cur_dist;
		}
		if(getGyroDegrees(gyroSensor)<360){
			turn_left();
			}else{
			turn_right();
		}
	}
}

void other_wall(){
	if(cur_dist< 25){
		while(cur_dist< 25){
			setMotorSync(leftMotor, rightMotor, 0, -20);
		}
		}else{
		resetGyro(gyroSensor);
		while(getGyroDegrees(gyroSensor)<180){
			setMotorSync(leftMotor, rightMotor, 0, -5);
		}
	}
	dist1=cur_dist;
	cur_dist=255;
	resetMotorEncoder(leftMotor);
	while(cur_dist>=255){
		setMotorSync(leftMotor, rightMotor, 0, 20);
	}
	int leftgonedeg=getMotorEncoder(leftMotor);
	int totalleft=(leftgonedeg*0.51);
	dist2=cur_dist;
	total_dist=totalleft+dist2+dist1;


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
		cur_dist=average();

	}
}

task display(){
	while (true){
		displayBigTextLine(10,"%d", cur_dist);
	}
}

task main(){
	startTask(display,7);
	startTask (update_dist,7);
	sleep(1000);
	//find_wall();
	//displayBigTextLine(8,"after wall");
	square();
	displayBigTextLine(8,"square");
	//other_wall();
	//displayBigTextLine(8,"%d",total_dist);
	while (true){}



}
