#pragma config(StandardModel, "EV3_REMBOT");
float speed;
float slewrate;
float totalleft=0;
float totalright=0;
float distance;

void decrease(){
	if (speed-slewrate>=15){
		speed-=slewrate*1.5;
		}else{
		speed=10;
	}
	sleep(250);
}

void increase(){
	if (speed+slewrate<=100){
		speed+=slewrate;
		}else{
		speed=100;
	}
	sleep(250);
}

void go(float dist){
	distance=dist;
	while (true){
		if(distance-totalleft<=(distance/2) || distance-totalright<=(distance/2)){ //half way
				decrease();
		}
		else{
			increase();
		}
		if(totalleft<distance || totalright<distance){
			setMotorSpeed(leftMotor, speed);
			setMotorSpeed(rightMotor, speed);
		}
		if (totalleft>=distance || totalright>=distance){
			stopAllTasks();
		}
	}
}

void display(){
displayBigTextLine(6,"speed: %f",speed);
displayBigTextLine(8," left: %f",totalleft);
displayBigTextLine(10,"right: %f",totalright);
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
		totalleft+=(leftgonedeg*0.51);
		totalright+=(rightgonedeg*0.51);
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
go(304.8);

}
task trav(){
distancetravled();
}

task main(){
speed=0;
slewrate=3;
startTask (displayTask,10);
startTask (test,10);
startTask (trav,10);
while(true){}
}
