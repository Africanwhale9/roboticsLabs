#pragma config(StandardModel, "EV3_REMBOT");
float speed;
float slewrate;
float totalleft=0;
float totalright=0;
float distance;
float disttomax=((((.136*speed+2.4)*speed)*(slewrate/3))*2);

void decrease(){
	if (speed-slewrate>=15){
		speed-=slewrate;
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

//void slew(){
//	while(true){
//		disttomax=((((.136*speed)+2.4)*speed)*2)*(slewrate/3);
//		if(distance-totalleft<=((((.136*speed)+2.4)*speed)*2)*(3/slewrate) || distance-totalright<=((((.136*speed)+2.4)*speed)*2)*(slewrate/3)){
//			decrease();
//			sleep(3000);
//			}else{
//			increase();
//		}
//	}
//}
void slew(){
	while(true){
		disttomax=((((.136*speed)+2.4)*speed)*2)*(slewrate/3);
		if(totalleft>= distance/2){
			if(distance-totalleft<=disttomax || distance-totalright<=disttomax){
				decrease();
			}
		}else{
			increase();
		}
	}
}

void go(float dist){
	distance=dist;
	while (true){
		if(totalleft<distance || totalright<distance){
			setMotorSpeed(leftMotor, speed);
			setMotorSpeed(rightMotor, speed);
		}
		if (totalleft>=distance || totalright>=distance){
			speed=0;
			stopAllTasks();
		}
	}
}

void display(){
	displayBigTextLine(6,"speed: %f",speed);
	displayBigTextLine(8," left: %f",totalleft);
	displayBigTextLine(10,"right: %f",totalright);
	displayBigTextLine(12,"STOP: %f", ((((.136*speed+2.4)*speed)*(slewrate/3))));
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
task slewTask(){
	slew();
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
	startTask (slewTask,10);
	while(true){}
}
