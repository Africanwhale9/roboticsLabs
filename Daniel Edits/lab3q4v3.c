#pragma config(StandardModel, "EV3_REMBOT");
float speed;
float slewrate;
float totalleft=0;
float totalright=0;
float distance;


void go(){
	if(totalleft<distance || totalright<distance){
		setMotorSpeed(leftMotor, speed);
		setMotorSpeed(rightMotor, speed);
		}else{
		stopAllTasks();
	}

}
void display(){
	displayBigTextLine(6,"speed: %f",speed);
	displayBigTextLine(8," left: %f",totalleft);
	displayBigTextLine(10,"right: %f",totalright);
}

void increase(){
	if (speed+slewrate<=100){
		speed+=slewrate;
		}else{
		speed=100;
	}
	sleep(250);
}

void decrease(){
	if (speed-slewrate>=0){
		speed-=slewrate;
		}else{
		speed=0;
	}
	sleep(250);
}

void distancetravled(){
	float leftgonedeg;
	float rightgonedeg;
	int reset=0;
	while (true){
		leftgonedeg=getMotorEncoder(leftMotor);
		rightgonedeg=getMotorEncoder(rightMotor);
		if(reset>=200){
			reset=0;
			totalleft+=(leftgonedeg*0.5);
			totalright+=(rightgonedeg*0.5);
			resetMotorEncoder(leftMotor);
			resetMotorEncoder(rightMotor);
		}
		reset+=1;
	}
}

task displayTask(){
	while (true){
		display();
	}
}
task test(){
	while (true){
		go();
	}
}
task trav(){
	distancetravled();
}

task main(){
	speed=10;
	slewrate=3;
	distance=304.8;
	startTask (displayTask,10);
	startTask (test,10);
	startTask (trav,10);
	while(true){}
}
