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

void reset(){
	speed=0;
	slewrate=3;
	totalleft=0;
	totalright=0;
	stops=true;
}

task test(){
	distance=reqdist;
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
task slewTask(){
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
task trav(){
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

task foward(){
	startTask (trav,10);
	startTask (test,10);
	startTask (slewTask,10);
	while (true){
		if (!stops){
			stopTask(test);
			stopTask(trav);
			stopTask(slewTask);
			reset();
			return;
		}
	}
}
void plswork(float dist){
	reqdist=dist;
	startTask(foward,10);
}

task main(){
	slewrate=3;
	plswork(300);
	sleep(10000);
	plswork(200);
	sleep(10000);
	//foward(200);
	//reset();
	//foward(300);

	while(true){}
}
