#pragma config(StandardModel, "EV3_REMBOT");
float speed;
float slewrate;
float totalleft=0;
float totalright=0;
float distance;


void decrease(){
	if (distance>=1){
		if (speed-slewrate>=7){
			speed-=slewrate;
			}else{
			speed=7;
		}
		sleep(250);
		}else{
		if (speed+slewrate<=-7){
			speed+=slewrate;
			}else{
			speed=-7;
		}
		sleep(250);
	}
}

void increase(){
	if (distance>=1){
		if (speed+slewrate<=100){
			speed+=slewrate;
			}else{
			speed=100;
		}
		sleep(250);
		}else{
		if (speed-slewrate>=-100){
			speed-=slewrate;
			}else{
			speed=-100;
		}
		sleep(250);
	}
}
void slew(){
	while(true){
		if(distance>=1){
			if(distance-totalleft<=((((.136*speed)+2.4)*speed)*2)*(3/slewrate) || distance-totalright<=((((.136*speed)+2.4)*speed)*2)*(slewrate/3)){
				while (true){
					if(distance-totalleft<=((((.136*speed)+2.4)*speed)*2)*(3/slewrate) || distance-totalright<=((((.136*speed)+2.4)*speed)*2)*(slewrate/3)){
						decrease();
					}
				}
				}else{
				increase();
			}
			}else{
			if(distance-totalleft>=((((.136*speed)-2.4)*speed)*2)*(3/slewrate)*(-1) || distance-totalright>=((((.136*speed)-2.4)*speed)*2)*(slewrate/3)*(-1)){
				while (true){
					if(distance-totalleft>=((((.136*speed)-2.4)*speed)*2)*(3/slewrate)*(-1) || distance-totalright>=((((.136*speed)-2.4)*speed)*2)*(slewrate/3)*(-1)){
						displayBigTextLine(4,"dec:");
						decrease();
					}
				}
				}else{
				displayBigTextLine(4,"in:");
				increase();
			}
		}
	}
}

void go(float dist){
	distance=dist;
	while (true){
		if(distance>=1){
			if(totalleft<distance || totalright<distance){
				setMotorSpeed(leftMotor, speed);
				setMotorSpeed(rightMotor, speed);
			}
			if (totalleft>=distance || totalright>=distance){
				speed=0;
				stopAllTasks();
			}
			}else{
			if(totalleft>distance || totalright>distance){
				setMotorSpeed(leftMotor, speed);
				setMotorSpeed(rightMotor, speed);
			}
			if (totalleft<=distance || totalright<=distance){
				speed=0;
				stopAllTasks();
			}
		}
	}
}

void display(){
	displayBigTextLine(6,"speed: %f",speed);
	displayBigTextLine(8," left: %f",totalleft);
	displayBigTextLine(10,"right: %f",totalright);
	displayBigTextLine(12,"STOP: %f", ((((.136*speed+2.4)*speed)*(slewrate/3)*(-1))));
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
	go(-804.8);
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
