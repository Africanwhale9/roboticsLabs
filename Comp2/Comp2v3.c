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



//float desiredDeg;
//float currentDeg;
//float error;
//int desiredPower=25;
//int currentPower;
//int slewRate = 1;
//float k =1;
//float tolerance = 1;

//task slew(){
//	while (true){
//		error = desiredDeg - currentDeg;
//		if(abs(error)>tolerance){//this should take care of the direction automatically
//			desiredPower = k*error*-1;//this will be negative if currentDist>disireddist (must flip the sign)
//			}else{
//			desiredPower=0;
//		}
//		if(error<0){//error calculation already tells us which way the robot needs to move, if error is negative, it must move forward
//			if((currentPower + slewRate) < desiredPower){
//				currentPower += slewRate;
//				}else{
//				currentPower = desiredPower;
//				}if (currentPower> desiredPower){
//				currentPower = desiredPower;
//			}
//		}
//	}
//}

//task update(){
//	while (true){
//		currentDeg=getGyroDegrees(gyroSensor);
//	}
//}

//task motorControl(){
//	while(true){
//		setMotorSync(leftMotor, rightMotor, -100, desiredPower);
//		if(getGyroDegrees(gyroSensor)>=desiredDeg){
//			stopTask(slew);
//			stopTask(update);
//			stopTask(motorControl);
//		}

//	}
//}



//void turn(int deg){
//	currentDeg= getGyroDegrees(gyroSensor);
//	desiredDeg=deg+currentDeg;
//	startTask(slew,7);
//	startTask(motorControl,7);
//	startTask(update,7);
//}

//void stop(){
//	setMotorSync (leftMotor, rightMotor, 0, 0);
//}

//void find_wall(){
//	while (cur_dist>=max_dist)	{
//		setMotorSync(leftMotor, rightMotor, 0, 20);
//	}
//	stop();
//}



//void orient(){
//	bool stop = false;
//	float tmp=cur_dist;
//	int degrees = 2;
//	float short_dist;
//	while(!stop){
//		turn(degrees);

//		if(cur_dist<=tmp){
//			turn(degrees);

//		}

//		else{
//			degrees = degrees*-1;
//			turn(degrees);
//			stop=true;
//		}

//		if(cur_dist<short_dist){
//			short_dist=cur_dist;
//		}


//		tmp=cur_dist;
//	}
//}



//void square(){
//	float old_dist=cur_dist;
//	bool stops=false;
//	//turn(1);
//	if(old_dist>cur_dist){ // turn correct way
//		while (!stops){
//			turn(1);
//			if(old_dist<cur_dist){
//				turn(-1);
//				stops=true;
//			}
//			old_dist=cur_dist;
//		}
//	}
//	else{
//		while (!stops){
//			turn(-1);
//			if(old_dist<cur_dist){
//				turn(1);
//				stops=true;
//			}
//			old_dist=cur_dist;
//		}
//	}
//}
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

void other_wall(){
	dist1=cur_dist;
	setMotorSyncEncoder(leftMotor, rightMotor, 100, 360, 100);

	resetMotorEncoder(leftMotor);
	cur_dist=255;
	while(cur_dist>=25){
		setMotorSync(leftMotor, rightMotor, 0, 20);
	}
	int leftgonedeg=getMotorEncoder(leftMotor);
	int totalleft=(leftgonedeg*0.51);
	dist2=cur_dist;
	total_dist=totalleft+dist2+dist1;
	setMotorSyncEncoder(leftMotor, rightMotor, 0, total_dist/.51, 20);

}



task main(){
	startTask(display,7);
	startTask (update_dist,7);
	sleep(1000);
	find_wall();
	//displayBigTextLine(12,"found wall");
	//square();
	//orient();
	other_wall();

	while(true){}

}
