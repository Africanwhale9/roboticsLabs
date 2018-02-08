#pragma config(StandardModel, "EV3_REMBOT");
float speed=0;
float slewrate;
float totalleft=0;
float totalright=0;
float distance;
float reqdist;
bool stops=true;



void decrease(){
	if (speed-slewrate>=7){
		speed-=slewrate;
		}else{
		speed=7;
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
void slew(){
	while(true){
		if(distance-totalleft<=((((.136*speed)+2.4)*speed)*2)*(3/slewrate) || distance-totalright<=((((.136*speed)+2.4)*speed)*2)*(slewrate/3)){
			while (true){
				if(distance-totalleft<=((((.136*speed)+2.4)*speed)*2)*(3/slewrate) || distance-totalright<=((((.136*speed)+2.4)*speed)*2)*(slewrate/3)){
					decrease();
				}
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
			stops=false;
		}
	}
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
void reset(){
	speed=0;
	slewrate=3;
	totalleft=0;
	totalright=0;
	stops=true;
}

task test(){
	go(reqdist);
}
task slewTask(){
	slew();
}
task trav(){
	distancetravled();
}

void foward(float dist){
	reqdist=dist;
	startTask (test,10);
	startTask (trav,10);
	startTask (slewTask,10);
	while (stops){
	}
	stopTask(test);
	stopTask(trav);
	stopTask(slewTask);

}


task main(){
	slewrate=3;
	foward(200);
	reset();
	foward(300);

	while(true){}
}
